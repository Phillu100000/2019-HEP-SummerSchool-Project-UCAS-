#include "GaudiKernel/DeclareFactoryEntries.h"

#include "McDecayModeSvc/McDecayModeSvc.h"

DECLARE_SERVICE_FACTORY( McDecayModeSvc )

DECLARE_FACTORY_ENTRIES( McDecayModeSvc ) { 
  DECLARE_SERVICE( McDecayModeSvc );
}
