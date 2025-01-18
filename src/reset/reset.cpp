#include "reset.hpp"
extern "C"
{
  #include "libopencm3/cm3/scb.h"
}

NOINIT volatile eResetReason cReset::ResetNumber = E_NO_RESET;
NOINIT volatile ui16 cReset::ResetType;

cReset::cReset(void)
{
}

void cReset::SoftwareReset(const eResetReason arg_resetNumber, const ui16 arg_resetType)
{
  ResetNumber = arg_resetNumber;
  ResetType = arg_resetType;
  scb_reset_system();
}
