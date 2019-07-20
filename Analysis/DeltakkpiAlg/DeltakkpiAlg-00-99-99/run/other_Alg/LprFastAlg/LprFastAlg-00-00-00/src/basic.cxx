//ucas-PRLi (lipeirong11@mails.gucas.ac.cn)
#include "LprFastAlg/basic.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "McTruth/McParticle.h"
#include "BestDTagSvc/BestDTagSvc.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/Event.h"
#include "EventModel/EventHeader.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "EvtRecEvent/EvtRecEtaToGG.h"
#include "DstEvent/TofHitStatus.h"
#include "MdcRecEvent/RecMdcKalTrack.h"

#include "TRandom.h"
#include "TMath.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Geometry/Point3D.h"

#include "VertexFit/Helix.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/KalmanKinematicFit.h"
#include "SimplePIDSvc/ISimplePIDSvc.h"

#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "ParticleID/ParticleID.h"

//#include "LTagTool/LTagTool.h" 
//#include "DTagTool/DTagTool.h" 

using CLHEP::Hep3Vector;
using CLHEP::Hep2Vector;
using CLHEP::HepLorentzVector;
#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif
#include <vector>
typedef std::vector<int> Vint;
typedef std::vector<double> Vdou;
typedef std::vector<HepLorentzVector> Vp4;
typedef std::vector<WTrackParameter> VWTrkPara;

using namespace Event;

const double xmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};//0=electron 1=muon 2=pion 3=kaon 4=proton
const double mpi0 =  0.134976;
const double mpion = 0.13957;
const double mkaon = 0.493677;
const double mproton = 0.938272;
//const double msgm0=1.192642;//pdg
const double msgm0=1.19255;//pdt_table
const double msgmP=1.18937;//pdt_table
const double msgmM=1.197436;//pdt_table
const double mlmd=1.115683;
const double m_Ds=1.96847;

int N_total(0),N_cut1(0);

///////////////////////////////////////////////////////////////
basic::basic(const std::string& name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator)
{
		declareProperty("HaveEle",           m_HaveEle =0);
		declareProperty("HaveKaon",           m_HaveKaon =0);
		declareProperty("HavePion",           m_HavePion =0);
		declareProperty("HaveProton",           m_HaveProton =0);
		declareProperty("HaveKs",           m_HaveKs =0);
		declareProperty("HaveLambda",           m_HaveLambda =0);
		declareProperty("HavePi0",           m_HavePi0 =0);
		declareProperty("HaveEta",           m_HaveEta =0);
		declareProperty("Debug",           m_debug =0);
		declareProperty("CheckTotal",      m_checktotal=0);

		//good track
		declareProperty("Vr0cut",   m_vr0cut=1.0);
		declareProperty("Vz0cut",   m_vz0cut=10.0);
		declareProperty("CosThetaCut", m_CosThetaCut = 0.93);

		//good shower
		declareProperty("PhotonMinEnergy", m_minEnergy = 0.025);
		declareProperty("GammaAngleCut", m_gammaAngleCut=20.0);
		declareProperty("GammatlCut",   m_gammatlCut=0.0);
		declareProperty("GammathCut",  m_gammathCut=14.0);
		declareProperty("PhotonMaxCosThetaBarrel", m_maxCosThetaBarrel = 0.8);
		declareProperty("PhotonMinCosThetaEndcap", m_minCosThetaEndcap = 0.84);
		declareProperty("PhotonMaxCosThetaEndcap", m_maxCosThetaEndcap = 0.92);
		declareProperty("PhotonMinEndcapEnergy",   m_minEndcapEnergy   = 0.050);
		//
		declareProperty("ecms",   m_ecms = 4.6);
}

StatusCode basic::initialize(){

		MsgStream log(msgSvc(), name());

		log << MSG::INFO << "in initialize()" << endmsg;

		StatusCode status;

		if(m_checktotal){
				NTuplePtr nt1(ntupleSvc(), "FILE_STag/tree_truth");
				if ( nt1 ) m_tuple1 = nt1;
				else {
						m_tuple1 = ntupleSvc()->book ("FILE_STag/tree_truth", CLID_ColumnWiseTuple, "tree_truth N-Tuple example");
						if ( m_tuple1 )    {
								status = m_tuple1->addItem ("runNo",       m_runNo_);
								status = m_tuple1->addItem ("evtNo",       m_evtNo_);
								status = m_tuple1->addItem ("mode1",       m_mode1_);
								status = m_tuple1->addItem ("mode2",       m_mode2_);
								status = m_tuple1->addItem ("mode3",       m_mode3_);
								status = m_tuple1->addItem ("ndaughterAp",       m_ndaughterAp_, 0, 15);
								status = m_tuple1->addIndexedItem ("Ap_id", m_ndaughterAp_, m_Ap_id_);
								status = m_tuple1->addIndexedItem ("Ap_ptruth", m_ndaughterAp_,  4, m_Ap_ptruth_);
								status = m_tuple1->addItem ("ndaughterAm",       m_ndaughterAm_, 0, 15);
								status = m_tuple1->addIndexedItem ("Am_id", m_ndaughterAm_, m_Am_id_);
								status = m_tuple1->addIndexedItem ("Am_ptruth", m_ndaughterAm_,  4, m_Am_ptruth_);
						}
						else    { 
								log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple1) << endmsg;
								return StatusCode::FAILURE;
						}
				}
		}

		NTuplePtr nt2(ntupleSvc(), "FILE_STag/tree");
		if ( nt2 ) m_tuple_tree = nt2;
		else {
				m_tuple_tree = ntupleSvc()->book ("FILE_STag/tree", CLID_ColumnWiseTuple, "tree N-Tuple example");
				if ( m_tuple_tree )    {
						status = m_tuple_tree->addItem ("evtNo",       m_evtNo);
						status = m_tuple_tree->addItem ("runNo",       m_runNo);
						status = m_tuple_tree->addItem ("mode1",       m_mode1);
						status = m_tuple_tree->addItem ("mode2",       m_mode2);
						status = m_tuple_tree->addItem ("mode3",       m_mode3);
						status = m_tuple_tree->addItem ("indexmc",  m_mcparticle, 0, 100);
						status = m_tuple_tree->addIndexedItem ("pdgid",   m_mcparticle,  m_pdgid);
						status = m_tuple_tree->addIndexedItem ("motheridx",   m_mcparticle,  m_motheridx);
						status = m_tuple_tree->addItem ("indexmc_p",  m_mcparticle_p, 0, 100);
						status = m_tuple_tree->addIndexedItem ("pdgid_p",   m_mcparticle_p,  m_pdgid_p);
						status = m_tuple_tree->addIndexedItem ("motheridx_p",   m_mcparticle_p,  m_motheridx_p);
						status = m_tuple_tree->addItem ("indexmc_m",  m_mcparticle_m, 0, 100);
						status = m_tuple_tree->addIndexedItem ("pdgid_m",   m_mcparticle_m,  m_pdgid_m);
						status = m_tuple_tree->addIndexedItem ("motheridx_m",   m_mcparticle_m,  m_motheridx_m);
						status = m_tuple_tree->addItem ("ndaughterAp",       m_ndaughterAp, 0, 15);
						status = m_tuple_tree->addIndexedItem ("Ap_id", m_ndaughterAp, m_Ap_id);
						status = m_tuple_tree->addIndexedItem ("Ap_ptruth", m_ndaughterAp,  4, m_Ap_ptruth);
						status = m_tuple_tree->addItem ("ndaughterAm",       m_ndaughterAm, 0, 15);
						status = m_tuple_tree->addIndexedItem ("Am_id", m_ndaughterAm, m_Am_id);
						status = m_tuple_tree->addIndexedItem ("Am_ptruth", m_ndaughterAm,  4, m_Am_ptruth);
						//****************************************************************** add 4 moment for all mode

						status = m_tuple_tree->addItem ("rightflag", m_rightflag);

						status = m_tuple_tree->addItem ("proton_p4", 4, m_proton_p4);
						status = m_tuple_tree->addItem ("Ks_p4",  4, m_Ks_p4);
						status = m_tuple_tree->addItem ("Eta_p4",  4, m_Eta_p4);

						status = m_tuple_tree->addItem ("Ks_mass",  m_Ks_mass);
						status = m_tuple_tree->addItem ("Ks_chis1",  m_Ks_chis1);
						status = m_tuple_tree->addItem ("Ks_lchue",  m_Ks_lchue);
						status = m_tuple_tree->addItem ("Eta_mass",  m_Eta_mass);
						status = m_tuple_tree->addItem ("Eta_chis",  m_Eta_chis);

						status = m_tuple_tree->addItem ("deltaE",  m_deltaE);
						status = m_tuple_tree->addItem ("mbc",  m_mbc);

				}
				else    {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_tree) << endmsg;
						return StatusCode::FAILURE;
				}
		}
		//--------end of book--------
		log << MSG::INFO << "successfully return from initialize()" <<endmsg;
		return StatusCode::SUCCESS;
}
//***********************************************************************
StatusCode basic::execute() {

		N_total++;
		m_rightflag=-1;
		MsgStream log(msgSvc(), name());
		log << MSG::INFO << "in execute()" << endreq;

		SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
		int runNo=eventHeader->runNumber();
		int eventNo=eventHeader->eventNumber();

		//if(runNo!= -30184 || eventNo!= 19) return StatusCode::SUCCESS;
		//cout<<__LINE__<<"**** run:event *****   "<<runNo<<"***"<<eventNo<<endl;

		//**************************LPR:TRUTH part***********************
		int mm_mode1=((eventHeader->flag1()/1000000))%1000;
		int mm_mode2=(eventHeader->flag1()/1000)%1000 ;
		int mm_mode3=eventHeader->flag1()%1000;

		//cout<<__LINE__<<"**** mode1:mode2:mode3*****   "<<mm_mode1<<"***"<<mm_mode2<<"***"<<mm_mode3<<endl;
		//if(mm_mode1!= 0 || mm_mode2!= 1) return StatusCode::SUCCESS;
		//****topology
		IMcDecayModeSvc* i_svc;
		StatusCode sc_DecayModeSvc = service("McDecayModeSvc", i_svc);
		if ( sc_DecayModeSvc.isFailure() ){
				log << MSG::FATAL << "Could not load McDecayModeSvc!" << endreq;
				return sc_DecayModeSvc;
		}
		m_svc = dynamic_cast<McDecayModeSvc*>(i_svc);

		int M_pdgid[100];
		int M_motheridx[100];

		int M_pdgid_p[100];
		int M_motheridx_p[100];

		int M_pdgid_m[100];
		int M_motheridx_m[100];

		int numParticle = 0;
		int numParticle_p = 0;
		int numParticle_m = 0;

		int ndaughterAp=0; double	Ap_ptruth[15][4]; int Ap_id[15];
		for ( int aa = 0; aa < 15; aa++ ) 
				for ( int ll = 0; ll < 4; ll++ ) 
						Ap_ptruth[aa][ll]=0;
		for ( int aa = 0; aa < 15; aa++ ) 
				Ap_id[aa]=0;

		int	ndaughterAm=0; double	Am_ptruth[15][4]; int Am_id[15];
		for ( int aa = 0; aa < 15; aa++ ) 
				for ( int ll = 0; ll < 4; ll++ ) 
						Am_ptruth[aa][ll]=0;
		for ( int aa = 0; aa < 15; aa++ ) 
				Am_id[aa]=0;

		//truth info
		if (eventHeader->runNumber()<0)
		{
				SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
				if (!mcParticleCol)
				{
						std::cout << "Could not retrieve McParticelCol" << std::endl;
						return StatusCode::FAILURE;
				}
				else {
						std::vector<int> pdgid;
						std::vector<int> motherindex;
						pdgid.clear();
						motherindex.clear();
						bool Lmdc_P=false;       bool Lmdc_M=false;
						Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
						for(; iter_mc != mcParticleCol->end(); iter_mc++){
								if ((*iter_mc)->primaryParticle()) continue;
								if (!(*iter_mc)->decayFromGenerator()) continue;
								int pdg = (*iter_mc)->particleProperty();
								int motherpdg = ((*iter_mc)->mother()).particleProperty();
								int mmotherpdg = (((*iter_mc)->mother()).mother()).particleProperty();
								if ((*iter_mc)->particleProperty()==4122){
										Lmdc_P=true;
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if ((*iter_mc)->particleProperty()==-4122){
										Lmdc_M=true;
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								//mode2
								if((mm_mode2==0||mm_mode2==2||mm_mode2==3)&&pdg==310&&motherpdg==-311&&mmotherpdg==4122){//anti-K0->Ks->pi+pi-
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode2>4&&mm_mode2<10&&pdg==3122&&motherpdg==4122){//Lambda->ppi-
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode2==10||mm_mode2==11)&&pdg==3212&&motherpdg==4122){//Sgm0->lambda gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode2==10||mm_mode2==11)&&pdg==3122&&motherpdg==3212&&mmotherpdg==4122){//Sgm0->lambda gam,  Lambda->ppi-
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode2>11&&mm_mode2<15&&pdg==3222&&motherpdg==4122){//Sgm+ ->p pi0
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode2==14&&pdg==223&&motherpdg==4122){//omega ->pip pim pi0
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode2==2||mm_mode2==4||mm_mode2==6||mm_mode2==12)&&pdg==111&&motherpdg==4122){//pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode2==12||mm_mode2==13||mm_mode2==14)&&pdg==111&&motherpdg==3222&&mmotherpdg==4122){////Sgm+ ->p pi0, pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode2==14&&pdg==111&&motherpdg==223&&mmotherpdg==4122){//omega ->pip pim pi0, pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Ap_id[ndaughterAp]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Ap_ptruth[ndaughterAp][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAp++;
										}// End of "gc.size() > 0" IF
								}


								//mode3
								if((mm_mode3==0||mm_mode3==2||mm_mode3==3)&&pdg==310&&motherpdg==311&&mmotherpdg==-4122){//K0->Ks->pi+pi-
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode3>4&&mm_mode3<10&&pdg==-3122&&motherpdg==-4122){//anti-Lambda->pbar pi+
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode3==10||mm_mode3==11)&&pdg==-3212&&motherpdg==-4122){//anti-Sgm0->anti-lambda gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode3==10||mm_mode3==11)&&pdg==-3122&&motherpdg==-3212&&mmotherpdg==-4122){//anti-Sgm0->anti-lambda gam,anti-Lambda->pbar pi+
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode3>11&&mm_mode3<15&&pdg==-3222&&motherpdg==-4122){//Sgm- ->pbar pi0
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode3==14&&pdg==223&&motherpdg==-4122){//omega ->pip pim pi0
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode3==2||mm_mode3==4||mm_mode3==6||mm_mode3==12)&&pdg==111&&motherpdg==-4122){//pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if((mm_mode3==12||mm_mode3==13||mm_mode3==14)&&pdg==111&&motherpdg==-3222&&mmotherpdg==-4122){////Sgm+ ->p pi0, pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}
								if(mm_mode3==14&&pdg==111&&motherpdg==223&&mmotherpdg==-4122){//omega ->pip pim pi0, pi0->gam gam
										const SmartRefVector<Event::McParticle>& gc = (*iter_mc)->daughterList();
										for(unsigned int ii = 0; ii < gc.size(); ii++) {
												if( gc[ii]->particleProperty() == -22) continue;
												Am_id[ndaughterAm]=gc[ii]->particleProperty();
												for( int ll = 0; ll < 4; ll++ ) Am_ptruth[ndaughterAm][ll]=gc[ii]->initialFourMomentum()[ll];
												ndaughterAm++;
										}// End of "gc.size() > 0" IF
								}


						}
						if(m_debug)  cout<<"Lmdc_P="<<Lmdc_P<<" Lmdc_M="<<Lmdc_M<<endl;

						for (iter_mc = mcParticleCol->begin(); iter_mc != mcParticleCol->end(); iter_mc++){
								if ((*iter_mc)->primaryParticle()) continue;
								if (!(*iter_mc)->decayFromGenerator()) continue;
								//trace psi(4415)
								if ((*iter_mc)->particleProperty()==9020443 || (*iter_mc)->particleProperty()==9030443 ){
										int mode = m_svc->extract(*iter_mc, pdgid, motherindex);
										numParticle = pdgid.size();
										for (int i=0; i!=  pdgid.size(); i++ ) {
												M_pdgid[i] = pdgid[i];
												if(m_debug) cout<<"M_pdgid[i]="<<M_pdgid[i]<<endl;
												M_motheridx[i] = motherindex[i];
												if(m_debug) cout<<"M_motheridx[i]="<<M_motheridx[i]<<endl;
										}
								}
								pdgid.clear();
								motherindex.clear();
								//trace Lambda_C+
								if ((*iter_mc)->particleProperty()==4122){
										int mode = m_svc->extract(*iter_mc, pdgid, motherindex);
										numParticle_p = pdgid.size();
										for (int i=0; i!=  pdgid.size(); i++ ) {
												M_pdgid_p[i] = pdgid[i];
												if(m_debug) cout<<"M_pdgid_p[i]="<<M_pdgid_p[i]<<endl;
												M_motheridx_p[i] = motherindex[i];
												if(m_debug) cout<<"M_motheridx_p[i]="<<M_motheridx_p[i]<<endl;
										} 
								} 
								pdgid.clear();
								motherindex.clear();
								//trace Lambda_C-
								if ((*iter_mc)->particleProperty()==-4122){
										int mode = m_svc->extract(*iter_mc, pdgid, motherindex);
										numParticle_m = pdgid.size();
										for (int i=0; i!=  pdgid.size(); i++ ) {
												M_pdgid_m[i] = pdgid[i];
												if(m_debug) cout<<"M_pdgid_m[i]="<<M_pdgid_m[i]<<endl;
												M_motheridx_m[i] = motherindex[i];
												if(m_debug) cout<<"M_motheridx_m[i]="<<M_motheridx_m[i]<<endl;
										}
								}
						}
				}
		}//end truth info



		Vint pdgid; pdgid.clear();
		Vint motherindex; motherindex.clear();

		if (eventHeader->runNumber()<0)
		{
				SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");

				if (!mcParticleCol)
				{
						std::cout << "Could not retrieve McParticelCol" << std::endl;
						// return StatusCode::FAILURE;
				}
				else
				{

						Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
						for (; iter_mc != mcParticleCol->end(); iter_mc++)
						{
								if ((*iter_mc)->primaryParticle()) continue;
								if (!(*iter_mc)->decayFromGenerator()) continue;
								if ((*iter_mc)->particleProperty()==9020443||(*iter_mc)->particleProperty()==9030443||(*iter_mc)->particleProperty()==9040443)//psi(4415)=9020443; psi(4260)=9030443;psi(4360)=9040443
										int mode = m_svc->extract(*iter_mc, pdgid, motherindex);
						}

				} 
		}


		if(m_checktotal){
				m_runNo_=runNo;
				m_evtNo_=eventNo;
				m_mode1_=mm_mode1;
				m_mode2_=mm_mode2;
				m_mode3_=mm_mode3;
				m_ndaughterAp_= ndaughterAp;
				for ( int aa = 0; aa < ndaughterAp; aa++ ) m_Ap_id_[aa]=Ap_id[aa];
				for ( int aa = 0; aa < ndaughterAp; aa++ ) 
						for ( int ll = 0; ll < 4; ll++ ) 
								m_Ap_ptruth_[aa][ll]=Ap_ptruth[aa][ll];

				m_ndaughterAm_= ndaughterAm;
				for ( int aa = 0; aa < ndaughterAm; aa++ ) m_Am_id_[aa]=Am_id[aa];
				for ( int aa = 0; aa < ndaughterAm; aa++ ) 
						for ( int ll = 0; ll < 4; ll++ ) 
								m_Am_ptruth_[aa][ll]=Am_ptruth[aa][ll];

				m_tuple1->write();
		}



		//**************************LPR:Good Track/shower***********************
		SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);

		SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid())
		{
				double* dbv = vtxsvc->PrimaryVertex(); 
				double*  vv = vtxsvc->SigmaPrimaryVertex();  
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}



		Vint iGood_loose; iGood_loose.clear();
		Vint ipionm_loose,ipionp_loose; ipionm_loose.clear(); ipionp_loose.clear();
		Vint iprotonm_loose,iprotonp_loose; iprotonm_loose.clear(); iprotonp_loose.clear();


		Vint iGood; iGood.clear();
		Vint iTrkm,iTrkp; iTrkm.clear(); iTrkp.clear();
		Vint ielem,ielep; ielem.clear(); ielep.clear();
		Vint ipionm,ipionp; ipionm.clear(); ipionp.clear();
		Vint ikaonm,ikaonp; ikaonm.clear(); ikaonp.clear();
		Vint iprotonm,iprotonp; iprotonm.clear(); iprotonp.clear();



		Vp4 p4elem,p4elep; p4elem.clear(); p4elep.clear();
		Vp4 p4pionm,p4pionp; p4pionm.clear(); p4pionp.clear();
		Vp4 p4kaonm,p4kaonp; p4kaonm.clear(); p4kaonp.clear();
		Vp4 p4protonm,p4protonp; p4protonm.clear(); p4protonp.clear();


		int nCharge = 0;
		for(int i = 0; i < evtRecEvent->totalCharged(); i++)
		{
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
				RecMdcTrack *mdcTrk = (*itTrk)->mdcTrack();
				RecMdcKalTrack* mdcKalTrk = (*itTrk)->mdcKalTrack();


				if(!isGoodTrackForKsLambda(*itTrk)) continue;

				iGood_loose.push_back(i);

				if(mdcTrk->charge()==1) 
				{
						//if(ispion()) ipionp_loose.push_back(i);
						ipionp_loose.push_back(i);
						iprotonp_loose.push_back(i);
						//if(isproton()) iprotonp_loose.push_back(i);
				}

				if(mdcTrk->charge()==-1)
				{
						//if(ispion()) ipionm_loose.push_back(i);
						ipionm_loose.push_back(i);
						iprotonm_loose.push_back(i);
						//if(isproton()) iprotonm_loose.push_back(i);
				}


				if(!isGoodTrack(*itTrk)) continue;
				iGood.push_back(i);

				preparePID(*itTrk);
				//**************************LPR:PID***********************
				//				ISimplePIDSvc* m_simplePIDSvc;
				//				Gaudi::svcLocator()->service("SimplePIDSvc", m_simplePIDSvc);
				//				m_simplePIDSvc->preparePID(*itTrk);
				//				if( m_simplePIDSvc->ispion() )


				if(mdcTrk->charge()==1) 
				{
						iTrkp.push_back(i);
						if(m_HaveEle&&iselectron()) {
								ielep.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::electron);
								p4elep.push_back(mdcKalTrk->p4(xmass[0]));
						}
						if(m_HavePion&&ispion()) {
								ipionp.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::pion);
								p4pionp.push_back(mdcKalTrk->p4(xmass[2]));
						}
						if(m_HaveKaon&&iskaon()){
								ikaonp.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::kaon);
								p4kaonp.push_back(mdcKalTrk->p4(xmass[3]));
						}
						if(m_HaveProton&&isproton()) {
								iprotonp.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::proton);
								p4protonp.push_back(mdcKalTrk->p4(xmass[4]));
						}
				}

				if(mdcTrk->charge()==-1)
				{
						iTrkm.push_back(i);
						if(m_HaveEle&&iselectron()){
								ielem.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::electron);
								p4elem.push_back(mdcKalTrk->p4(xmass[0]));
						}
						if(m_HavePion&&ispion()){
								ipionm.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::pion);
								p4pionm.push_back(mdcKalTrk->p4(xmass[2]));
						}
						if(m_HaveKaon&&iskaon()) {
								ikaonm.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::kaon);
								p4kaonm.push_back(mdcKalTrk->p4(xmass[3]));
						}
						if(m_HaveProton&&isproton()){
								iprotonm.push_back(i);
								mdcKalTrk->setPidType(RecMdcKalTrack::proton);
								p4protonm.push_back(mdcKalTrk->p4(xmass[4]));
						}
				}


				nCharge += mdcTrk->charge();
		}

		// Finish Good Charged Track Selection
		int nGood = iGood.size();
		int nTrkp = iTrkp.size();
		int nelep = ielep.size();
		int npionp = ipionp.size();
		int nkaonp = ikaonp.size();
		int nprotonp = iprotonp.size();

		int nTrkm = iTrkm.size();
		int nelem = ielem.size();
		int npionm = ipionm.size();
		int nkaonm = ikaonm.size();
		int nprotonm = iprotonm.size();

		if(m_debug){
				cout<<"nGood=  "<<nGood<<endl;
				cout<<"nTrkp=  "<<nTrkp<<";    nTrkm = "<<nTrkm<<"; "<<endl;
				cout<<"nelep=  "<<nelep<<";    nelem = "<<nelem<<"; "<<endl;
				cout<<"npionp=  "<<npionp<<";    npionm = "<<npionm<<"; "<<endl;
				cout<<"nkaonp=  "<<nkaonp<<";    nkaonm = "<<nkaonm<<"; "<<endl;
				cout<<"nprotonp=  "<<nprotonp<<";    nprotonm = "<<nprotonm<<"; "<<endl;
		}

		if(nGood != 3 && nGood != 4) return StatusCode::SUCCESS;
		if(fabs(nCharge)>1) return StatusCode::SUCCESS;

		//***************************************************

		Vint iGam; iGam.clear();
		Vp4 p4Gam; p4Gam.clear();

		for(int i = evtRecEvent->totalCharged(); i< evtRecEvent->totalTracks(); i++){
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;

				if(!isGoodShower(*itTrk)) continue;
				iGam.push_back(i);

				RecEmcShower *emcTrk = (*itTrk)->emcShower();
				p4Gam.push_back(getP4(emcTrk,xorigin));
		}
		// Finish Good Photon Selection

		int nGam = iGam.size();

		if(m_debug){
				cout<<"nGam=  "<<nGam<<endl;
		}
		if(nGam<2) return StatusCode::SUCCESS;


		//**************************LPR:PI0/Eta***********************
		Vint iPi0gam1,iPi0gam2;iPi0gam1.clear(),iPi0gam2.clear();
		Vdou massPi0; massPi0.clear();
		Vdou chisPi0; chisPi0.clear();
		Vp4 p4Pi0; p4Pi0.clear();
		Vp4 p4Pi01c; p4Pi01c.clear();
		int nPi0;

		if(m_HavePi0){
				// Loop each gamma pair, check if it is a pi0
				for(int i = 0; i < nGam-1; i++) 
				{
						for(int j = i+1; j < nGam; j++) 
						{
								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iGam[i];
								RecEmcShower* shr1 = (*itTrki)->emcShower();

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + iGam[j];
								RecEmcShower* shr2 = (*itTrkj)->emcShower();

								HepLorentzVector p4_pi0(0,0,0,0),p4_pi0_1c(0,0,0,0);
								double pi0_mass;
								double pi0_chis;

								if(isGoodpi0(shr1,shr2,pi0_mass,p4_pi0,pi0_chis,p4_pi0_1c)){
										iPi0gam1.push_back(iGam[i]);
										iPi0gam2.push_back(iGam[j]);
										massPi0.push_back(pi0_mass);
										chisPi0.push_back(pi0_chis);
										p4Pi0.push_back(p4_pi0);
										p4Pi01c.push_back(p4_pi0_1c);
								}

						}
				}
				nPi0 = iPi0gam1.size();
				if(nPi0==0) return StatusCode::SUCCESS;
		}


		Vint iEtagam1,iEtagam2;iEtagam1.clear(),iEtagam2.clear();
		Vdou massEta; massEta.clear();
		Vdou chisEta; chisEta.clear();
		Vp4 p4Eta; p4Eta.clear();
		Vp4 p4Eta1c; p4Eta1c.clear();
		int nEta;

		if(m_HaveEta){
				// Loop each gamma pair, check if it is a eta
				for(int i = 0; i < nGam-1; i++) 
				{
						for(int j = i+1; j < nGam; j++) 
						{
								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iGam[i];
								RecEmcShower* shr1 = (*itTrki)->emcShower();

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + iGam[j];
								RecEmcShower* shr2 = (*itTrkj)->emcShower();

								HepLorentzVector p4_eta(0,0,0,0),p4_eta_1c(0,0,0,0);
								double eta_mass;
								double eta_chis;

								if(isGoodeta(shr1,shr2,eta_mass,p4_eta,eta_chis,p4_eta_1c)){
										iEtagam1.push_back(iGam[i]);
										iEtagam2.push_back(iGam[j]);
										massEta.push_back(eta_mass);
										chisEta.push_back(eta_chis);
										p4Eta.push_back(p4_eta);
										p4Eta1c.push_back(p4_eta_1c);
								}

						}
				}
				nEta = iEtagam1.size();
				if(nEta==0) return StatusCode::SUCCESS;
		}


		//**************************LPR:Ks/Lambda***********************
		//Ks
		Vint iKspip,iKspim;iKspip.clear(),iKspim.clear();
		Vdou massKs; massKs.clear();
		Vdou chis1Ks; chis1Ks.clear();
		Vdou chis2Ks; chis2Ks.clear();
		Vdou lchueKs; lchueKs.clear();
		Vp4 p4Ks1s; p4Ks1s.clear();
		int nKs;

		if(m_HaveKs){
				// Loop each pi+ pi- pair, check if it is a Ks
				for(int i = 0; i < ipionp_loose.size(); i++) 
				{
						for(int j = 0; j < ipionm_loose.size(); j++) 
						{
								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + ipionp_loose[i];
								RecMdcKalTrack *pipKalTrk = (*itTrki)->mdcKalTrack();

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + ipionm_loose[j];
								RecMdcKalTrack *pimKalTrk = (*itTrkj)->mdcKalTrack();

								HepLorentzVector p4_ks_1s(0,0,0,0);
								double ks_1chis,ks_2chis,ks_lchue,ks_mass;

								if(isGoodks(pipKalTrk, pimKalTrk, ks_1chis, ks_2chis, ks_lchue, p4_ks_1s, ks_mass)){

										iKspip.push_back(ipionp_loose[i]);
										iKspim.push_back(ipionm_loose[j]);
										massKs.push_back(ks_mass);
										chis1Ks.push_back(ks_1chis);
										chis2Ks.push_back(ks_2chis);
										lchueKs.push_back(ks_lchue);
										p4Ks1s.push_back(p4_ks_1s);
								}

						}
				}
				nKs = iKspip.size();
				if(nKs==0) return StatusCode::SUCCESS;
		}

		//Lmd
		Vint iLmdpp,iLmdpim;iLmdpp.clear(),iLmdpim.clear();
		Vdou massLmd; massLmd.clear();
		Vdou chis1Lmd; chis1Lmd.clear();
		Vdou chis2Lmd; chis2Lmd.clear();
		Vdou lchueLmd; lchueLmd.clear();
		Vp4 p4Lmd1s; p4Lmd1s.clear();
		int nLmd;

		Vint iLmdpm,iLmdpip;iLmdpm.clear(),iLmdpip.clear();
		Vdou massALmd; massALmd.clear();
		Vdou chis1ALmd; chis1ALmd.clear();
		Vdou chis2ALmd; chis2ALmd.clear();
		Vdou lchueALmd; lchueALmd.clear();
		Vp4 p4ALmd1s; p4ALmd1s.clear();
		int nALmd;

		if(m_HaveLambda){
				// Loop each p pi- pair, check if it is a lambda
				for(int i = 0; i < iprotonp_loose.size(); i++) 
				{
						for(int j = 0; j < ipionm_loose.size(); j++) 
						{
								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iprotonp_loose[i];
								RecMdcKalTrack *ppKalTrk = (*itTrki)->mdcKalTrack();

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + ipionm_loose[j];
								RecMdcKalTrack *pimKalTrk = (*itTrkj)->mdcKalTrack();

								HepLorentzVector p4_lmd_1s(0,0,0,0);
								double lmd_1chis,lmd_2chis,lmd_lchue,lmd_mass;

								if(isGoodlmd(ppKalTrk, pimKalTrk, lmd_1chis, lmd_2chis, lmd_lchue, p4_lmd_1s, lmd_mass)){

										iLmdpp.push_back(iprotonp_loose[i]);
										iLmdpim.push_back(ipionm_loose[j]);
										massLmd.push_back(lmd_mass);
										chis1Lmd.push_back(lmd_1chis);
										chis2Lmd.push_back(lmd_2chis);
										lchueLmd.push_back(lmd_lchue);
										p4Lmd1s.push_back(p4_lmd_1s);
								}

						}
				}

				// Loop each pbar pi+ pair, check if it is a Anti-lambda
				for(int i = 0; i < iprotonm_loose.size(); i++) 
				{
						for(int j = 0; j < ipionp_loose.size(); j++) 
						{
								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iprotonm_loose[i];
								RecMdcKalTrack *pmKalTrk = (*itTrki)->mdcKalTrack();

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + ipionp_loose[j];
								RecMdcKalTrack *pipKalTrk = (*itTrkj)->mdcKalTrack();

								HepLorentzVector p4_lmd_1s(0,0,0,0);
								double lmd_1chis,lmd_2chis,lmd_lchue,lmd_mass;

								if(isGoodlmd(pmKalTrk, pipKalTrk, lmd_1chis, lmd_2chis, lmd_lchue, p4_lmd_1s, lmd_mass)){

										iLmdpm.push_back(iprotonm_loose[i]);
										iLmdpip.push_back(ipionp_loose[j]);
										massALmd.push_back(lmd_mass);
										chis1ALmd.push_back(lmd_1chis);
										chis2ALmd.push_back(lmd_2chis);
										lchueALmd.push_back(lmd_lchue);
										p4ALmd1s.push_back(p4_lmd_1s);
								}

						}
				}

				nLmd = iLmdpp.size();
				nALmd = iLmdpm.size();
				if(nLmd==0&&nALmd==0) return StatusCode::SUCCESS;
		}


		//	// what we have now!
		//	Vint ipionm_loose,ipionp_loose; 
		//	Vint iprotonm_loose,iprotonp_loose;

		//	Vint ielem,ielep;              		Vp4 p4elem,p4elep; 
		//	Vint ipionm,ipionp;            		Vp4 p4pionm,p4pionp; 
		//	Vint ikaonm,ikaonp;            		Vp4 p4kaonm,p4kaonp; 
		//	Vint iprotonm,iprotonp;        		Vp4 p4protonm,p4protonp; 
		//	Vint iGam;                     		Vp4 p4Gam;                  


		//	Vint iPi0gam1,iPi0gam2;    		Vint iEtagam1,iEtagam2;
		//	Vdou massPi0;              		Vdou massEta; 
		//	Vdou chisPi0;              		Vdou chisEta;
		//	Vp4 p4Pi0;                 		Vp4 p4Eta; 
		//	Vp4 p4Pi01c;               		Vp4 p4Eta1c;
		//	int nPi0;                  		int nEta;                    

		//	//Ks                    		//Lmd
		//	Vint iKspip,iKspim;     		Vint iLmdpp,iLmdpim;  		Vint iLmdpm,iLmdpip;
		//	Vdou massKs;            		Vdou massLmd;         		Vdou massALmd; 
		//	Vdou chis1Ks;           		Vdou chis1Lmd;        		Vdou chis1ALmd;
		//	Vdou chis2Ks;           		Vdou chis2Lmd;        		Vdou chis2ALmd;
		//	Vdou lchueKs;           		Vdou lchueLmd;        		Vdou lchueALmd;
		//	Vp4 p4Ks1s;             		Vp4 p4Lmd1s;          		Vp4 p4ALmd1s;
		//	int nKs;                		int nLmd;             		int nALmd;                 


		// For vertex fit!
		// p, kaon-, pion+
		double VFchi2=9999;
		double KFchi2=9999;
		Vp4 p4fit_charge, p4fit_photon;
		p4fit_charge.clear(); p4fit_photon.clear();


		for(int i = 0; i < iprotonp.size(); i++) 
		{
				for(int j = 0; j < ikaonm.size(); j++) 
				{
						for(int k = 0; k < ipionp.size(); k++) 
						{
								if(iprotonp[i]==ipionp[k]) continue;

								double iVFchi2=9999;
								VWTrkPara vwtrkpara_charge; vwtrkpara_charge.clear(); 
								VertexParameter birth;

								EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iprotonp[i];
								RecMdcKalTrack *KalTrki = (*itTrki)->mdcKalTrack();
								KalTrki->setPidType(RecMdcKalTrack::proton);
								vwtrkpara_charge.push_back( WTrackParameter(xmass[4], KalTrki->getZHelixP(), KalTrki->getZErrorP()) );

								EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + ikaonm[j];
								RecMdcKalTrack *KalTrkj = (*itTrkj)->mdcKalTrack();
								KalTrkj->setPidType(RecMdcKalTrack::kaon);
								vwtrkpara_charge.push_back( WTrackParameter(xmass[3], KalTrkj->getZHelixK(), KalTrkj->getZErrorK()) );

								EvtRecTrackIterator itTrkk = evtRecTrkCol->begin() + ipionp[k];
								RecMdcKalTrack *KalTrkk = (*itTrkk)->mdcKalTrack();
								KalTrkk->setPidType(RecMdcKalTrack::pion);
								vwtrkpara_charge.push_back( WTrackParameter(xmass[2], KalTrkk->getZHelix(), KalTrkk->getZError()) );

								if(!pass_vf(vwtrkpara_charge,  birth, iVFchi2)) continue;

								for(int l = 0; l < nPi0; l++) 
								{
										VWTrkPara vwtrkpara_photon; vwtrkpara_photon.clear(); 

										EvtRecTrackIterator itTrki = evtRecTrkCol->begin() + iPi0gam1[l];
										RecEmcShower* shr1 = (*itTrki)->emcShower();

										EvtRecTrackIterator itTrkj = evtRecTrkCol->begin() + iPi0gam2[l];
										RecEmcShower* shr2 = (*itTrkj)->emcShower();

										HepLorentzVector g1P4 = getP4(shr1,xorigin);
										HepLorentzVector g2P4 = getP4(shr2,xorigin);

										vwtrkpara_photon.push_back( WTrackParameter(shr1->position(), g1P4, shr1->dphi(), shr1->dtheta(), shr1->dE()) );
										vwtrkpara_photon.push_back( WTrackParameter(shr2->position(), g2P4, shr2->dphi(), shr2->dtheta(), shr2->dE()) );

										double iKFchi2=9999;
										Vp4 ip4fit_charge, ip4fit_photon; ip4fit_charge.clear(); ip4fit_photon.clear();
										if(!pass_kf(vwtrkpara_charge, vwtrkpara_photon, birth, iKFchi2, ip4fit_charge, ip4fit_photon)) continue;

										if(iKFchi2 < KFchi2)
										{
												KFchi2 = iKFchi2;
												p4fit_charge=ip4fit_charge;
												p4fit_photon=ip4fit_photon;
										}
								}
						}
				}
		}



		double ebeam=2.3;                             

		double deltaE_min=9999;
		double mBC=9999;
		int index_i;
		int index_j;
		int index_k;

		// p, Ks,eta , select the best one
		for(int i = 0; i < iprotonp.size(); i++) 
		{
				for(int j = 0; j < nKs; j++) 
				{
						for(int k = 0; k <nEta; k++) 
						{
								if(iprotonp[i]==iKspip[j]) continue;
								HepLorentzVector pD0 =	p4protonp[i]+ p4Ks1s[j] + p4Eta1c[k];
								pD0.boost(-0.011,0,0);
								double deltaE = pD0.t() - ebeam;
								double mbc2 = ebeam*ebeam - pD0.v().mag2();
								double mbc = mbc2 > 0 ? sqrt( mbc2 ) : -10;
								if (fabs(deltaE)<fabs(deltaE_min))
								{
										deltaE_min=deltaE;
										mBC=mbc;
										index_i=i;
										index_j=j;
										index_k=k;
								}

						}
				}
		}


		//after give value for all branch 
		for ( int jj = 0; jj < 4; jj++ ) m_proton_p4[jj] = p4protonp[index_i][jj];
		for ( int jj = 0; jj < 4; jj++ ) m_Ks_p4[jj] = p4Ks1s[index_j][jj];
		for ( int jj = 0; jj < 4; jj++ ) m_Eta_p4[jj] = p4Eta1c[index_k][jj];
		m_Ks_mass=massKs[index_j];
		m_Ks_chis1=chis1Ks[index_j];
		m_Ks_lchue=lchueKs[index_j];
		m_Eta_mass=massEta[index_k];
		m_Eta_chis=chisEta[index_k];

		m_deltaE=deltaE_min;
		m_mbc=mBC;
		m_rightflag=1;
		m_tuple1->write();


		// p, Ks,eta , keep all 
		for(int i = 0; i < iprotonp.size(); i++) 
		{
				for(int j = 0; j < nKs; j++) 
				{
						for(int k = 0; k <nEta; k++) 
						{
								if(iprotonp[i]==iKspip[j]) continue;
								HepLorentzVector pD0 =	p4protonp[i]+ p4Ks1s[j] + p4Eta1c[k];
								pD0.boost(-0.011,0,0);
								double deltaE = pD0.t() - ebeam;
								double mbc2 = ebeam*ebeam - pD0.v().mag2();
								double mbc = mbc2 > 0 ? sqrt( mbc2 ) : -10;

								//after give value for all branch 
								for ( int jj = 0; jj < 4; jj++ ) m_proton_p4[jj] = p4protonp[i][jj];
								for ( int jj = 0; jj < 4; jj++ ) m_Ks_p4[jj] = p4Ks1s[j][jj];
								for ( int jj = 0; jj < 4; jj++ ) m_Eta_p4[jj] = p4Eta1c[k][jj];

								m_Ks_mass=massKs[j];
								m_Ks_chis1=chis1Ks[j];
								m_Ks_lchue=lchueKs[j];
								m_Eta_mass=massEta[k];
								m_Eta_chis=chisEta[k];

								m_deltaE=deltaE;
								m_mbc=mbc;
								m_rightflag=1;
								m_tuple1->write();

						}
				}
		}


		return StatusCode::SUCCESS;
} //end of execute()

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode basic::finalize() {
		MsgStream log(msgSvc(), name());
		log << MSG::INFO << "in finalize()" << endmsg;
		//cout << "Ntotal  "<<Ntotal<<endl;
		//cout << "Ncut0   "<<Ncut0<<endl;
		//cout << "Ncut1   "<<Ncut1<<endl;
		//cout << "Ncut2   "<<Ncut2<<endl;
		//cout << "Ncut3   "<<Ncut3<<endl;
		//cout << "Ncut4   "<<Ncut4<<endl;
		//cout << "Ncut5   "<<Ncut5<<endl;
		return StatusCode::SUCCESS;
}

//************************************************************************
HepLorentzVector basic::getP4(RecEmcShower* gTrk, Hep3Vector origin){

		//double eraw = gTrk->energy();
		//double phi =  gTrk->phi();
		//double the =  gTrk->theta();

		//return HepLorentzVector( eraw * sin(the) * cos(phi),	eraw * sin(the) * sin(phi), 	eraw * cos(the),	eraw );


		Hep3Vector Gm_Vec(gTrk->x(), gTrk->y(), gTrk->z());
		Hep3Vector Gm_Mom = Gm_Vec - origin;
		Gm_Mom.setMag(gTrk->energy());
		HepLorentzVector pGm(Gm_Mom, gTrk->energy());
		return pGm; 
}

HepLorentzVector basic::cal_cms(const HepLorentzVector boost_p4, const HepLorentzVector cal_p4){
		HepLorentzVector tmp_p4(cal_p4);
		Hep3Vector boostVector = boost_p4.boostVector();
		tmp_p4.boost(-boostVector);
		return tmp_p4;
}

void basic::cal_helicity(const HepLorentzVector A_p4, const HepLorentzVector B_p4,const HepLorentzVector C_p4, double& Cda_B) // A:father B:me C:son
{
		HepLorentzVector A_B_p4 = cal_cms(B_p4,A_p4);
		HepLorentzVector C_B_p4 = cal_cms(B_p4,C_p4);
		Hep3Vector A_B_p3= A_B_p4.vect();
		//Cda_B = C_B_p4.angle(A_B_p3);
		Cda_B = cos(C_B_p4.angle(A_B_p3));
}
void basic::preparePID(EvtRecTrack* track){

		//PID for proton
		ParticleID *pid = ParticleID::instance();
		pid->init();
		pid->setMethod(pid->methodProbability());
		pid->setChiMinCut(8);
		pid->setRecTrack(track);
		pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTof()); // use PID sub-system
		//pid->identify(pid->onlyPion() | pid->onlyKaon());
		pid->identify(pid->onlyPion() | pid->onlyKaon() | pid->onlyProton());
		pid->calculate();
		m_prob[0]=pid->probElectron();
		m_prob[1]=pid->probMuon();
		m_prob[2]=pid->probPion();
		m_prob[3]=pid->probKaon();
		m_prob[4]=pid->probProton();
		if(!(pid->IsPidInfoValid())) {for(int i=0; i<5; i++) m_prob[i]=-99; }

		//PID for kaon and pion
		ParticleID *PID = ParticleID::instance();
		PID->init();
		PID->setMethod(PID->methodProbability());
		PID->setChiMinCut(4);
		PID->setRecTrack(track);
		PID->usePidSys(PID->useDedx() | PID->useTof1() | PID->useTof2() | PID->useTof()); // use PID sub-system
		PID->identify(PID->onlyPion() | PID->onlyKaon());
		//PID->identify(PID->onlyPion() | PID->onlyKaon() | PID->onlyProton());
		PID->calculate();
		mm_prob[0]=PID->probElectron();
		mm_prob[1]=PID->probMuon();
		mm_prob[2]=PID->probPion();
		mm_prob[3]=PID->probKaon();
		mm_prob[4]=PID->probProton();
		if(!(PID->IsPidInfoValid())) {for(int i=0; i<5; i++) mm_prob[i]=-99; }


		//PID for electron 
		ParticleID *Epid = ParticleID::instance();
		Epid->init();
		Epid->setMethod(Epid->methodProbability());
		Epid->setChiMinCut(4);
		Epid->setRecTrack(track);
		Epid->usePidSys(Epid->useDedx() | Epid->useTof1() | Epid->useTof2() | Epid->useTof() | Epid->useEmc()); // use PID sub-system
		Epid->identify(Epid->onlyElectron() | Epid->onlyPion() | Epid->onlyKaon());
		Epid->calculate();
		mmm_prob[0]=Epid->probElectron();
		mmm_prob[1]=Epid->probMuon();
		mmm_prob[2]=Epid->probPion();
		mmm_prob[3]=Epid->probKaon();
		mmm_prob[4]=Epid->probProton();
		if(!(Epid->IsPidInfoValid())) {for(int i=0; i<5; i++) mmm_prob[i]=-99; }


		RecMdcTrack *mdcTrk = track->mdcTrack();

		m_eop=-1;
		if(track->isEmcShowerValid())
		{
				RecEmcShower *emcTrk = track->emcShower();

				m_eop = (emcTrk->energy())/(mdcTrk->p());
		}


}

bool basic::iselectron(){

		if(mmm_prob[0]>0.001 && (mmm_prob[0]/(mmm_prob[0]+mmm_prob[2]+mmm_prob[3]))>0.8){ return true; }
		return false;
}

bool basic::iselectron_eop(){

		if(m_eop>0.8) return true;

		return false;
}

bool basic::isproton(){
		if(m_prob[4]>=0 && m_prob[4]>=m_prob[3] && m_prob[4]>=m_prob[2]){ return true; }
		return false;
}

bool basic::ispion(){

		if(mm_prob[2]>=0.00 && mm_prob[2]>=mm_prob[3]) return true;
		return false;
}

bool basic::iskaon(){

		if(mm_prob[3]>=0.00 && mm_prob[3]>=mm_prob[2]){ return true; }
		return false;
}



bool basic::isGoodTrack(EvtRecTrack* trk){

		if( !trk->isMdcKalTrackValid()) {
				return false;
		}

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double*  vv = vtxsvc->SigmaPrimaryVertex();
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}


		RecMdcTrack *mdcTrk = trk->mdcTrack();

		HepVector a = mdcTrk->helix();
		HepSymMatrix Ea = mdcTrk->err();

		HepPoint3D point0(0.,0.,0.); // the initial point for MDC recosntruction
		HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]);
		VFHelix helixip(point0,a,Ea);
		helixip.pivot(IP);
		HepVector vecipa = helixip.a();
		double  Rvxy0=fabs(vecipa[0]);  //the nearest distance to IP in xy plane
		double  Rvz0=vecipa[3];         //the nearest distance to IP in z direction
		double costheta=cos(mdcTrk->theta());

		if(fabs(Rvz0) < m_vz0cut && fabs(Rvxy0)< m_vr0cut  &&fabs(costheta)<m_CosThetaCut)  return true;

		return false;
}


bool basic::isGoodShower(EvtRecTrack* trk){

		if(!trk->isEmcShowerValid()){
				return false;
		}


		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double*  vv = vtxsvc->SigmaPrimaryVertex();
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}

		SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);

		SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);


		RecEmcShower *emcTrk = trk->emcShower();

		Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z());
		HepLorentzVector shP4 = getP4(emcTrk,xorigin);
		double cosThetaSh = shP4.vect().cosTheta();
		double eraw = emcTrk->energy();
		double getTime = emcTrk->time();
		if(getTime>m_gammathCut||getTime<m_gammatlCut) return false;
		if(!((fabs(cosThetaSh) < m_maxCosThetaBarrel&&eraw > m_minEnergy)||((fabs(cosThetaSh) > m_minCosThetaEndcap)&&(fabs(cosThetaSh) < m_maxCosThetaEndcap)&&(eraw > m_minEndcapEnergy))))  return false;

		double dang = 200.; 
		for(int j = 0; j < evtRecEvent->totalCharged(); j++)
		{
				EvtRecTrackIterator jtTrk = evtRecTrkCol->begin() + j; 
				if(!(*jtTrk)->isExtTrackValid()) continue;
				RecExtTrack *extTrk = (*jtTrk)->extTrack();
				if(extTrk->emcVolumeNumber() == -1) continue;
				Hep3Vector extpos = extTrk->emcPosition();
				double angd = extpos.angle(emcpos);
				if(angd < dang) dang = angd;
		}
		//if(dang>=200) return false;
		dang = dang * 180 / (CLHEP::pi);
		if(fabs(dang) < m_gammaAngleCut) return false;

		return true;
}

bool basic::isGoodpi0(RecEmcShower *shr1,RecEmcShower *shr2,double& pi0_mass,HepLorentzVector& p4_pi0,double& pi0_chis,HepLorentzVector& p4_pi0_1c){

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double*  vv = vtxsvc->SigmaPrimaryVertex();
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}

		pi0_mass=-100;
		pi0_chis=-100;

		HepLorentzVector g1P4 = getP4(shr1,xorigin);
		HepLorentzVector g2P4 = getP4(shr2,xorigin);
		p4_pi0 = g1P4 + g2P4;
		pi0_mass = p4_pi0.m();

		if(!(pi0_mass > 0.08 && pi0_mass< 0.18)) return false;

		double xmpi0=0.134976;

		KalmanKinematicFit * kmfit = KalmanKinematicFit::instance();
		kmfit->init();
		kmfit->setIterNumber(5);
		kmfit->AddTrack(0, 0.0, shr1);
		kmfit->AddTrack(1, 0.0, shr2);
		kmfit->AddResonance(0, xmpi0, 0, 1);

		bool olmdq =kmfit->Fit(0);
		if(!olmdq) return false;

		kmfit->BuildVirtualParticle(0);
		pi0_chis = kmfit->chisq(0);
		if(pi0_chis>2500) return false;

		p4_pi0_1c=kmfit->pfit(0)+kmfit->pfit(1);

		return true;
}



bool basic::isGoodeta(RecEmcShower *shr1,RecEmcShower *shr2,double& eta_mass,HepLorentzVector& p4_eta,double& eta_chis,HepLorentzVector& p4_eta_1c){

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double*  vv = vtxsvc->SigmaPrimaryVertex();
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}

		eta_mass=-100;
		eta_chis=-100;

		HepLorentzVector g1P4 = getP4(shr1,xorigin);
		HepLorentzVector g2P4 = getP4(shr2,xorigin);
		p4_eta = g1P4 + g2P4;
		eta_mass = p4_eta.m();

		if(!(eta_mass > 0.4 && eta_mass< 0.7)) return false;

		double xmeta=0.54784;

		KalmanKinematicFit * kmfit = KalmanKinematicFit::instance();
		kmfit->init();
		kmfit->setIterNumber(5);
		kmfit->AddTrack(0, 0.0, shr1);
		kmfit->AddTrack(1, 0.0, shr2);
		kmfit->AddResonance(0, xmeta, 0, 1);

		bool olmdq =kmfit->Fit(0);
		if(!olmdq) return false;

		kmfit->BuildVirtualParticle(0);
		eta_chis = kmfit->chisq(0);
		if(eta_chis>2500) return false;

		p4_eta_1c=kmfit->pfit(0)+kmfit->pfit(1);

		return true;
}




bool basic::isGoodTrackForKsLambda(EvtRecTrack* trk){

		if( !trk->isMdcKalTrackValid()) {
				return false;
		}

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double*  vv = vtxsvc->SigmaPrimaryVertex();
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}


		RecMdcTrack *mdcTrk = trk->mdcTrack();

		HepVector a = mdcTrk->helix();
		HepSymMatrix Ea = mdcTrk->err();

		HepPoint3D point0(0.,0.,0.); // the initial point for MDC recosntruction
		HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]);
		VFHelix helixip(point0,a,Ea);
		helixip.pivot(IP);
		HepVector vecipa = helixip.a();
		double  Rvxy0=fabs(vecipa[0]);  //the nearest distance to IP in xy plane
		double  Rvz0=vecipa[3];         //the nearest distance to IP in z direction
		double costheta=cos(mdcTrk->theta());

		if(fabs(Rvz0) < 20 && fabs(costheta)<m_CosThetaCut)  return true;

		return false;
}



bool basic::isGoodks(RecMdcKalTrack* pipTrk, RecMdcKalTrack* pimTrk, double& ks_1chis, double& ks_2chis, double& ks_lchue, HepLorentzVector& p4_ks_1s, double& ks_mass){
		ks_1chis=-100;
		ks_2chis=-100;
		ks_lchue =-100;
		ks_mass =-100;

		HepPoint3D vx(0., 0., 0.);
		HepSymMatrix Evx(3, 0);
		double bx = 1E+6;
		double by = 1E+6;
		double bz = 1E+6;
		Evx[0][0] = bx*bx;
		Evx[1][1] = by*by;
		Evx[2][2] = bz*bz;
		VertexParameter vxpar;
		vxpar.setVx(vx);
		vxpar.setEvx(Evx);

		VertexFit *vtxfit_s = VertexFit::instance();
		SecondVertexFit *vtxfit2 = SecondVertexFit::instance();
		RecMdcKalTrack::setPidType(RecMdcKalTrack::pion);
		WTrackParameter  wvpipksTrk, wvpimksTrk;
		wvpipksTrk = WTrackParameter(mpion, pipTrk->getZHelix(),pipTrk->getZError());
		wvpimksTrk = WTrackParameter(mpion, pimTrk->getZHelix(),pimTrk->getZError());

		//******primary vertex fit
		vtxfit_s->init();
		vtxfit_s->AddTrack(0, wvpipksTrk);
		vtxfit_s->AddTrack(1, wvpimksTrk);
		vtxfit_s->AddVertex(0, vxpar, 0, 1);
		bool okvs=vtxfit_s->Fit(0);

		if(!okvs) return false;

		vtxfit_s->Swim(0);
		vtxfit_s->BuildVirtualParticle(0);
		ks_1chis = vtxfit_s->chisq(0);
		WTrackParameter  wvks = vtxfit_s->wVirtualTrack(0);
		p4_ks_1s=wvks.p();

		ks_mass = p4_ks_1s.m();

		if(!(ks_mass > 0.46 && ks_mass< 0.54)) return false;


		WTrackParameter wpip = vtxfit_s->wtrk(0);
		WTrackParameter wpim = vtxfit_s->wtrk(1);
		//******second vertex fit
		HepPoint3D newvx(0., 0., 0.);
		HepSymMatrix newEvx(3, 0);
		VertexParameter primaryVpar;
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(!(vtxsvc->isVertexValid())) {
				cout<<"Attention --!(vtxsvc->isVertexValid())"<<endl;
		}
		double* db_vx = vtxsvc->PrimaryVertex();
		double* db_vx_err = vtxsvc->SigmaPrimaryVertex();
		newvx.setX(db_vx[0]);
		newvx.setY(db_vx[1]);
		newvx.setZ(db_vx[2]);
		newEvx[0][0] = db_vx_err[0]*db_vx_err[0];
		newEvx[1][1] = db_vx_err[1]*db_vx_err[1];
		newEvx[2][2] = db_vx_err[2]*db_vx_err[2];
		primaryVpar.setVx(newvx);
		primaryVpar.setEvx(newEvx);
		vtxfit2->init();
		vtxfit2->setPrimaryVertex(primaryVpar);
		vtxfit2->AddTrack(0, wvks);
		vtxfit2->setVpar(vtxfit_s->vpar(0));
		bool okv2=vtxfit2->Fit();
		if(!okv2) return false;

		ks_2chis= vtxfit2->chisq();
		//WTrackParameter wks;
		//wks = vtxfit2->wpar();
		//p4_ks_2s=wks.p();

		double ks_dl  = vtxfit2->decayLength();
		double ks_dle = vtxfit2->decayLengthError();
		ks_lchue = ks_dl/ks_dle;

		return true;
}



bool basic::isGoodlmd(RecMdcKalTrack* ppTrk, RecMdcKalTrack* pimTrk, double& lmd_1chis, double& lmd_2chis, double& lmd_lchue, HepLorentzVector& p4_lmd_1s, double& lmd_mass){
		lmd_1chis=-100;
		lmd_2chis=-100;
		lmd_lchue =-100;
		lmd_mass =-100;

		HepPoint3D vx(0., 0., 0.);
		HepSymMatrix Evx(3, 0);
		double bx = 1E+6;
		double by = 1E+6;
		double bz = 1E+6;
		Evx[0][0] = bx*bx;
		Evx[1][1] = by*by;
		Evx[2][2] = bz*bz;
		VertexParameter vxpar;
		vxpar.setVx(vx);
		vxpar.setEvx(Evx);

		VertexFit *vtxfit_s = VertexFit::instance();
		SecondVertexFit *vtxfit2 = SecondVertexFit::instance();

		WTrackParameter  wvpplmdTrk, wvpimlmdTrk;

		RecMdcKalTrack::setPidType(RecMdcKalTrack::proton);
		wvpplmdTrk = WTrackParameter(mproton, ppTrk->getZHelix(),ppTrk->getZError());

		RecMdcKalTrack::setPidType(RecMdcKalTrack::pion);
		wvpimlmdTrk = WTrackParameter(mpion, pimTrk->getZHelix(),pimTrk->getZError());

		//******primary vertex fit
		vtxfit_s->init();
		vtxfit_s->AddTrack(0, wvpplmdTrk);
		vtxfit_s->AddTrack(1, wvpimlmdTrk);
		vtxfit_s->AddVertex(0, vxpar, 0, 1);
		bool okvs=vtxfit_s->Fit(0);

		if(!okvs) return false;

		vtxfit_s->Swim(0);
		vtxfit_s->BuildVirtualParticle(0);
		lmd_1chis = vtxfit_s->chisq(0);
		WTrackParameter  wvlmd = vtxfit_s->wVirtualTrack(0);
		p4_lmd_1s=wvlmd.p();

		lmd_mass = p4_lmd_1s.m();

		if(!(lmd_mass > 1.09 && lmd_mass< 1.14)) return false;

		WTrackParameter wppFlmd = vtxfit_s->wtrk(0);
		WTrackParameter wpimFlmd = vtxfit_s->wtrk(1);
		//******second vertex fit
		HepPoint3D newvx(0., 0., 0.);
		HepSymMatrix newEvx(3, 0);
		VertexParameter primaryVpar;
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(!(vtxsvc->isVertexValid())) {
				cout<<"Attention --!(vtxsvc->isVertexValid())"<<endl;
		}
		double* db_vx = vtxsvc->PrimaryVertex();
		double* db_vx_err = vtxsvc->SigmaPrimaryVertex();
		newvx.setX(db_vx[0]);
		newvx.setY(db_vx[1]);
		newvx.setZ(db_vx[2]);
		newEvx[0][0] = db_vx_err[0]*db_vx_err[0];
		newEvx[1][1] = db_vx_err[1]*db_vx_err[1];
		newEvx[2][2] = db_vx_err[2]*db_vx_err[2];
		primaryVpar.setVx(newvx);
		primaryVpar.setEvx(newEvx);
		vtxfit2->init();
		vtxfit2->setPrimaryVertex(primaryVpar);
		vtxfit2->AddTrack(0, wvlmd);
		vtxfit2->setVpar(vtxfit_s->vpar(0));
		bool okv2=vtxfit2->Fit();
		if(!okv2) return false;

		lmd_2chis= vtxfit2->chisq();
		//WTrackParameter wlmd;
		//wlmd = vtxfit2->wpar();
		//p4_lmd_2s=wlmd.p();

		double lmd_dl  = vtxfit2->decayLength();
		double lmd_dle = vtxfit2->decayLengthError();
		lmd_lchue = lmd_dl/lmd_dle;

		return true;
}



//
bool basic::pass_vf(VWTrkPara &vwtrkpara, VertexParameter &birth, double &vf_chi2){

		vf_chi2=9999;

		HepPoint3D vx(0., 0., 0.);
		HepSymMatrix Evx(3, 0);
		double bx = 1E+6;
		double by = 1E+6;
		double bz = 1E+6;
		Evx[0][0] = bx*bx;
		Evx[1][1] = by*by;
		Evx[2][2] = bz*bz;
		VertexParameter vxpar;
		vxpar.setVx(vx);
		vxpar.setEvx(Evx);

		VertexFit* vtxfit = VertexFit::instance();

		vtxfit->init();

		for (int i=0; i < vwtrkpara.size(); i++){
				vtxfit->AddTrack(i,  vwtrkpara[i]);
		}

		std::vector<int> trkId(vwtrkpara.size(), 0);
		for(int i=0; i<vwtrkpara.size(); i++)  trkId[i]=i;

		vtxfit->AddVertex(0, vxpar, trkId);

		if(!vtxfit->Fit(0)) return false;

		vf_chi2 = vtxfit->chisq(0);

		vtxfit->Swim(0);

		for (int i=0; i < vwtrkpara.size(); i++){
				vwtrkpara[i]= vtxfit->wtrk(i);
		}

		birth=vtxfit->vpar(0);

		return true;
}


// test Kinematic fit
bool basic::pass_kf(const VWTrkPara &vwtrkpara_charge,const VWTrkPara &vwtrkpara_photon, const VertexParameter birth, double &kf_chi2, Vp4 &p4fit_charge, Vp4 &p4fit_photon){

		kf_chi2=999;

		KalmanKinematicFit * kmfit = KalmanKinematicFit::instance();

		kmfit->init();
		kmfit->setBeamPosition(birth.vx());
		kmfit->setVBeamPosition(birth.Evx());

		double ii=0;
		std::vector<int> listA, listB;
		while( ii<vwtrkpara_charge.size() ){
				kmfit->AddTrack(ii, vwtrkpara_charge[ii]);
				listA.push_back(ii);
				ii++;
		}
		while( ii<vwtrkpara_charge.size()+vwtrkpara_photon.size() ){
				kmfit->AddTrack(ii, vwtrkpara_photon[ii-vwtrkpara_charge.size()]);
				listB.push_back(ii);
				listA.push_back(ii);
				ii++;
		}

		kmfit->AddResonance(0, m_Ds, listA );

		for(int kk=0; kk<(listB.size()/2); kk++)  kmfit->AddResonance(kk+1, mpi0, vwtrkpara_charge.size()+kk*2, vwtrkpara_charge.size()+kk*2+1);

		HepLorentzVector ecms(0.011*4.6,0,0,4.6);
		kmfit->AddFourMomentum(3, ecms);

		if(!kmfit->Fit(0)) return false;
		if(!kmfit->Fit()) return false;

		//kf_chi2= kmfit->chisq();
		kf_chi2 = kmfit->chisq()/(1+listB.size()/2);

		for(int ii=0; ii<vwtrkpara_charge.size(); ii++)		p4fit_charge.push_back(kmfit->pfit(ii));
		for(int ii=vwtrkpara_charge.size(); ii<vwtrkpara_charge.size()+vwtrkpara_photon.size(); ii++)		p4fit_photon.push_back(kmfit->pfit(ii));

		return true;

}


