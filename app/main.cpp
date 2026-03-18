#include "systick.hpp"
#include "noinit_vars.hpp"
#include "errorhandler_inst.hpp"

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
}

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();

  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
  __asm__ volatile ("cpsie i");

  while (true)
  {
    
    gpio_toggle(GPIOB, GPIO6);
    cSysTick::DelayMs(1000);


    ErrorHandler.Cyclic();
  }

  return 0;
}
