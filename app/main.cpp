#include "systick.hpp"
#include "noinit_vars.hpp"
#include "errorhandler_inst.hpp"

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();

  while (true)
  {
    ErrorHandler.Cyclic();
  }

  return 0;
}
