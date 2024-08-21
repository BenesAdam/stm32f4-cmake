#include "noinit_vars.hpp"

namespace nsNoinitVars
{
  constexpr ui32 MagicNumberValue = 0xDEADBEEFU;
  constexpr ui32 InverseMagicNumberValue = ~MagicNumberValue;

  // Helper variables for first-run detection
  NOINIT static volatile ui32 magicNumber;
  NOINIT static volatile ui32 inverseMagicNumber;

  // Noinit variables
  NOINIT volatile ui16 ResetCount;

  // Set variables to 0 on cold start
  void InitializeOnColdStart(void)
  {
    const bool coldStartDetected = (magicNumber != MagicNumberValue) && (inverseMagicNumber != InverseMagicNumberValue);

    if(coldStartDetected)
    {
      // Variables to be initialized
      ResetCount = 0U;

      magicNumber = MagicNumberValue;
      inverseMagicNumber = InverseMagicNumberValue;
    }
  }
};
