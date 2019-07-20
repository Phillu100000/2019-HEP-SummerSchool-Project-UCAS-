#include <algorithm>
#include "GaudiKernel/MsgStream.h"
#include "McTruth/McParticle.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "McDecayModeSvc/DecayModes.h"
#include "McDecayModeSvc/PartId2Name.h"

using Event::McParticle;

McDecayModeSvc::McDecayModeSvc(const std::string& name, ISvcLocator* svcLoc)
  : Service(name, svcLoc), m_decayModes(0)
{
  declareProperty("pdt_file", m_pdt);
  declareProperty("NoTracingList", m_noTracing);
}

McDecayModeSvc::~McDecayModeSvc()
{
}

StatusCode McDecayModeSvc::initialize()
{
  MsgStream log(messageService(), name());
  log << MSG::INFO << "@initialize()" << endreq;

  StatusCode sc = Service::initialize();

  if (sc.isSuccess()) m_decayModes = new DecayModes();

  PartId2Name::instance(m_pdt);

  return sc;
}

StatusCode McDecayModeSvc::finalize()
{
  MsgStream log(messageService(), name());
  log << MSG::INFO << "@finalize()" << endreq;

  if (m_decayModes) delete m_decayModes;

  //PartId2Name* id2name = PartId2Name::instance();
  //if (id2name) delete id2name;
  PartId2Name::release();

  StatusCode sc = Service::finalize();
  return sc;
}

StatusCode McDecayModeSvc::queryInterface(const InterfaceID& riid, void** ppvIF)
{
  if (IMcDecayModeSvc::interfaceID().versionMatch(riid)) {
    *ppvIF = dynamic_cast<IMcDecayModeSvc*>(this);
  }
  else {
    return Service::queryInterface(riid, ppvIF);
  }
  addRef();
  return StatusCode::SUCCESS;
}

int McDecayModeSvc::extract(McParticle* prim)
{
  //MsgStream log(messageService(), name());

  DecayParticle* d_prim = new DecayParticle(prim->particleProperty());
  checkDecay(prim, d_prim);

  return m_decayModes->addMode(d_prim);
}

int McDecayModeSvc::extract(McParticle* prim, std::vector<int>& pdgid, std::vector<int>& motherindex )
{
  pdgid.clear();
  motherindex.clear();
  m_rootIndex = 0;
  m_pdgid.clear();
  m_motherindex.clear();
  m_rootIndex = prim->trackIndex();
  m_pdgid.push_back(prim->particleProperty() );
  m_motherindex.push_back((prim->mother() ).trackIndex() - m_rootIndex );

  DecayParticle* d_prim = new DecayParticle(prim->particleProperty());
  mycheckDecay(prim, d_prim);

  for (int i=0; i!= m_pdgid.size(); i++ ) {
  pdgid.push_back(m_pdgid[i]);
  motherindex.push_back(m_motherindex[i]);
  }
  return m_decayModes->addMode(d_prim);
}

void McDecayModeSvc::sum(McParticle* prim, int* sumOfParticle)
{
  //MsgStream log(messageService(), name());
//  int sumOfParticle[12];
//  DecayParticle* d_prim = new DecayParticle(prim->particleProperty());
  sumDecay(prim, sumOfParticle);
//  return sumOfParticle;
}

void McDecayModeSvc::summary(std::ostream& os) const {
  m_decayModes->printToStream(os);
}

void McDecayModeSvc::checkDecay(const McParticle* mother, DecayParticle* d_mother)
{
  if ( ! mother->leafParticle() ) {
    int id_tmp = mother->particleProperty();
    std::vector<int>::iterator it_tmp = find(m_noTracing.begin(), m_noTracing.end(), id_tmp);
    if ( it_tmp == m_noTracing.end() ) {
      const SmartRefVector<McParticle>& daughters = mother->daughterList();
      SmartRefVector<McParticle>::const_iterator it = daughters.begin();
      SmartRefVector<McParticle>::const_iterator end = daughters.end();

      for ( ; it != end; it++) {
	int d_id = (*it)->particleProperty();

	DecayParticle* d_daughter = d_mother->addDaughter(d_id);
	
	checkDecay(it->data(), d_daughter);
      }
    }
  }
}

void McDecayModeSvc::mycheckDecay(const McParticle* mother, DecayParticle* d_mother)
{
  if ( ! mother->leafParticle() ) {
  	int index_mother = m_pdgid.size() - 1;
	int id_tmp = mother->particleProperty();
	std::vector<int>::iterator it_tmp = find(m_noTracing.begin(), m_noTracing.end(), id_tmp);
	if ( it_tmp == m_noTracing.end() ) {
	  const SmartRefVector<McParticle>& daughters = mother->daughterList();
	  SmartRefVector<McParticle>::const_iterator it = daughters.begin();
	  SmartRefVector<McParticle>::const_iterator end = daughters.end();

	  for ( ; it != end; it++) {
		int d_id = (*it)->particleProperty();
		m_pdgid.push_back(d_id);
//		m_motherindex.push_back(mother->trackIndex() - m_rootIndex );
		m_motherindex.push_back(index_mother );
		DecayParticle* d_daughter = d_mother->addDaughter(d_id);

		mycheckDecay(it->data(), d_daughter);
	  }
	}
  }
}
void McDecayModeSvc::sumDecay(const McParticle* mother, int* sumOfParticle)
{
  std::vector<int> m_vParticle;
  m_vParticle.push_back(11);
  m_vParticle.push_back(-11);
  m_vParticle.push_back(13);
  m_vParticle.push_back(-13);
  m_vParticle.push_back(211);
  m_vParticle.push_back(-211);
  m_vParticle.push_back(321);
  m_vParticle.push_back(-321);
  m_vParticle.push_back(2212);
  m_vParticle.push_back(-2212);
  m_vParticle.push_back(111);
  m_vParticle.push_back(22);
  m_vParticle.push_back(310);
  if ( mother->decayFromGenerator() ) {
	int id_tmp = mother->particleProperty();
	std::vector<int>::iterator it_tmp = find(m_vParticle.begin(), m_vParticle.end(), id_tmp);
	if (it_tmp != m_vParticle.end() ) {
	  sumOfParticle[it_tmp-m_vParticle.begin()]++;
	  if (id_tmp == 310 )
	  {
		const SmartRefVector<McParticle>& daughters = mother->daughterList();
		SmartRefVector<McParticle>::const_iterator it = daughters.begin();
		SmartRefVector<McParticle>::const_iterator end = daughters.end();
		for ( ; it != end; it++) {
		  int d_id = (*it)->particleProperty();
		  sumDecay(it->data(), sumOfParticle);
		}
	  }
	}
	else {
	  if (! mother->leafParticle() ) {
		const SmartRefVector<McParticle>& daughters = mother->daughterList();
		SmartRefVector<McParticle>::const_iterator it = daughters.begin();
		SmartRefVector<McParticle>::const_iterator end = daughters.end();
		for ( ; it != end; it++) {
		  int d_id = (*it)->particleProperty();
		  sumDecay(it->data(), sumOfParticle);
		}
	  }
	}
  }
}

//map<std::vector<McParticle*>, int> McDecayModeSvc::getTruth(const McParticle* mother )
//{
////  map<int, std::vector<McParticle*> > m_mcVec;
//  std::vector<int> m_vParticle;
//  m_vParticle.push_back(11);
//  m_vParticle.push_back(-11);
//  m_vParticle.push_back(13);
//  m_vParticle.push_back(-13);
//  m_vParticle.push_back(211);
//  m_vParticle.push_back(-211);
//  m_vParticle.push_back(321);
//  m_vParticle.push_back(-321);
//  m_vParticle.push_back(2212);
//  m_vParticle.push_back(-2212);
//  m_vParticle.push_back(111);
//  m_vParticle.push_back(22);
//  m_vParticle.push_back(310);
//  if ( mother->decayFromGenerator() ) {
//	int id_tmp = mother->particleProperty();
//	std::vector<int>::iterator it_tmp = find(m_vParticle.begin(), m_vParticle.end(), id_tmp);
//	if (it_tmp != m_vParticle.end() ) {
//	  sumOfParticle[it_tmp-m_vParticle.begin()]++;
//	  m_mcVec.insert(make_pair(id_tmp, mother ) );
//	  if (id_tmp == 310 )
//	  {
//		const SmartRefVector<McParticle>& daughters = mother->daughterList();
//		SmartRefVector<McParticle>::const_iterator it = daughters.begin();
//		SmartRefVector<McParticle>::const_iterator end = daughters.end();
//		for ( ; it != end; it++) {
//		  int d_id = (*it)->particleProperty();
//		  getTruth(it->data(), sumOfParticle);
//		}
//	  }
//	}
//	else {
//	  if (! mother->leafParticle() ) {
//		const SmartRefVector<McParticle>& daughters = mother->daughterList();
//		SmartRefVector<McParticle>::const_iterator it = daughters.begin();
//		SmartRefVector<McParticle>::const_iterator end = daughters.end();
//		for ( ; it != end; it++) {
//		  int d_id = (*it)->particleProperty();
//		  getTruth(it->data(), sumOfParticle);
//		}
//	  }
//	}
//  }
//}
