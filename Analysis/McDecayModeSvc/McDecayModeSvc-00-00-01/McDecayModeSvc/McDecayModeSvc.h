#ifndef MC_DECAY_MODE_SVC_H
#define MC_DECAY_MODE_SVC_H

#include "GaudiKernel/Service.h"
#include "McDecayModeSvc/IMcDecayModeSvc.h"
#include <map>

template <class TYPE> class CnvFactory;

class DecayParticle;
class DecayModes;

class McDecayModeSvc : public Service, virtual public IMcDecayModeSvc
{
  friend class CnvFactory<McDecayModeSvc>;

public:
  McDecayModeSvc(const std::string& name, ISvcLocator* svcLoc);
  virtual ~McDecayModeSvc();

  virtual StatusCode initialize();
  virtual StatusCode finalize();
  virtual StatusCode queryInterface(const InterfaceID& riid, void** ppvIF);

  int  extract(Event::McParticle* prim);
  int  extract(Event::McParticle* prim, std::vector<int>& pdgid, std::vector<int>& motherindex);
  void sum(Event::McParticle* prim, int* sumOfParticle );
  void summary(std::ostream& os = std::cout) const;

private:
  void checkDecay(const Event::McParticle* mother, DecayParticle* d_mother);
  void mycheckDecay(const Event::McParticle* mother, DecayParticle* d_mother);
  void sumDecay(const Event::McParticle* mother, int* sumOfParticle );
//  std::vector<McParticle*> getTruth(const Event::McParticle* mother );

  DecayModes*      m_decayModes;
  std::string      m_pdt;
  std::vector<int> m_noTracing;
  int m_rootIndex;
  std::vector<int> m_pdgid;
  std::vector<int> m_motherindex;
//  map<int, std::vector<McParticle*> > m_mcVec;
//  map<int, std::vector<McParticle*> > m_mcVecKs;
};

#endif
