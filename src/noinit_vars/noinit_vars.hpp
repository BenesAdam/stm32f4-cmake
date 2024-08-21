#ifndef NOINIT_VARS_HPP
#define NOINIT_VARS_HPP

#include "types.h"

namespace nsNoinitVars
{
  extern volatile ui16 ResetCount;

  extern void InitializeOnColdStart(void);
};

#endif /* NOINIT_VARS_HPP */
