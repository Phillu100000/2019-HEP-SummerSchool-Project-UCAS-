#ifndef MC_DECAY_MODES_H
#define MC_DECAY_MODES_H

#include <list>
#include <iostream>
#include "McDecayModeSvc/DecayParticle.h"

class DecayModes
{
public:
  DecayModes();
  virtual ~DecayModes();

  int addMode(DecayParticle* mode);

  void printToStream(std::ostream& os);

private:
  class DecayMode;

  int                  m_totEvts;
  std::list<DecayMode> m_modes;
};

#endif
