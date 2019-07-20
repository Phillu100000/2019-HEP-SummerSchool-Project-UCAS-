#include "GaudiKernel/DeclareFactoryEntries.h"
#include "DeltakkpiAlg/Deltakkpi.h"
#include "DeltakkpiAlg/ReadBeamInfFromDb.h"

DECLARE_ALGORITHM_FACTORY( Deltakkpi )

DECLARE_FACTORY_ENTRIES( DeltakkpiAlg ) {
  DECLARE_ALGORITHM(Deltakkpi);
}

