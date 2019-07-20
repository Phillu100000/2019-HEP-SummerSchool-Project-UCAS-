#ifndef IMC_DECAY_MODE_SVC_H
#define IMC_DECAY_MODE_SVC_H

#include <iostream>
#include "GaudiKernel/IService.h"

/* Declaration of the interface ID */
static const InterfaceID IID_IMcDecayModeSvc("IMcDecayModeSvc", 1, 0);

namespace Event { class McParticle; }

class IMcDecayModeSvc : virtual public IService
{
public:
  virtual ~IMcDecayModeSvc() {}

  static const InterfaceID& interfaceID() { return IID_IMcDecayModeSvc; }

  virtual int  extract(Event::McParticle* prim) = 0;
  virtual void summary(std::ostream& os = std::cout) const = 0;
};

#endif
