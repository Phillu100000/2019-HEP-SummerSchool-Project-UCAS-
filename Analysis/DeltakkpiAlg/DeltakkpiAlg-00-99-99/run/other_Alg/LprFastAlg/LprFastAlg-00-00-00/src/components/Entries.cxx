#include "GaudiKernel/DeclareFactoryEntries.h"
#include "LprFastAlg/basic.h"
#include "LprFastAlg/PKPiCheckAlg.h"

DECLARE_ALGORITHM_FACTORY( basic )
DECLARE_ALGORITHM_FACTORY( PKPiCheckAlg )

DECLARE_FACTORY_ENTRIES( LprFastAlg ) {
DECLARE_ALGORITHM( basic );
DECLARE_ALGORITHM( PKPiCheckAlg );
}

