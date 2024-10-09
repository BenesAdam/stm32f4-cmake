#include "reset_react.hpp"

extern "C"
{
  #include "libopencm3/cm3/scb.h"
}

cResetReact::cResetReact(void)
{
}

void cResetReact::ErrorReaction(void)
{
  scb_reset_system();
}
