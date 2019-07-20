#include "McDecayModeSvc/DecayModes.h"

class DecayModes::DecayMode {
  public:
    DecayMode(DecayParticle* mode);
    bool operator<(DecayMode& other) { return nEvts > other.nEvts; }

    int            nEvts;
    int            modeId;
    DecayParticle* pMode;

  private:
    static int nTot;
};

int DecayModes::DecayMode::nTot = 0;

  DecayModes::DecayMode::DecayMode(DecayParticle* mode)
: nEvts(1), modeId(nTot), pMode(mode)
{
  nTot++;
}

  DecayModes::DecayModes()
: m_totEvts(0)
{
}

DecayModes::~DecayModes()
{
  std::list<DecayMode>::iterator it = m_modes.begin();
  for ( ; it != m_modes.end(); it++) delete it->pMode;
}

int DecayModes::addMode(DecayParticle* mode) {

  if ( ((++m_totEvts) & 0x3FF) == 0) m_modes.sort();

  std::list<DecayMode>::iterator it = m_modes.begin();
  std::list<DecayMode>::iterator end = m_modes.end();

  while ( it != end && (!mode->sameAs(it->pMode)) ) it++;

  if (it != end) {
    delete mode;
    it->nEvts++;
    return (it->modeId);
  }
  else {
    DecayMode d_mode(mode);
    m_modes.push_back(d_mode);
    return d_mode.modeId;
  }
}

void DecayModes::printToStream(std::ostream& os) {

  m_modes.sort();
  os << "************************************************************" << std::endl
    << "Total " << m_totEvts << " events in " << m_modes.size() << " decay modes" << std::endl
    << "************************************************************" << std::endl << std::endl;

  std::list<DecayMode>::const_iterator it = m_modes.begin();

  for (int i = 1; it != m_modes.end(); i++, it++ ) {
    os << i << "\t@@     modeId - " << it->modeId << ",\t total events: " << it->nEvts << std::endl
      << "------------------------------------------------------------" << std::endl;
    it->pMode->printToStream(os);
    os << "************************************************************" << std::endl << std::endl;
  }
}
