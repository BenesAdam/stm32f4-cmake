#include "systick.hpp"

extern "C"
{
#include "libopencm3/cm3/systick.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/nvic.h"
}

static volatile ui64 system_time = 0U;

extern "C" void sys_tick_handler(void)
{
  system_time++;
}

void cSysTick::Setup(void)
{
  // Configure general clock
#ifdef STM32F4
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);
#else
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
#endif

  // Configure system timer
  systick_set_frequency(cSysTick::Frequency, rcc_ahb_frequency);
  systick_counter_enable();
  systick_interrupt_enable();
}

// Returns number of microseconds
ui64 cSysTick::Micros(void)
{
  return system_time;
}

ui64 cSysTick::Millis(void)
{
  return cSysTick::Micros() / 1000ULL;
}

void cSysTick::DelayMs(ui64 milliseconds)
{
  const ui64 endTimeInMicros = cSysTick::Micros() + (milliseconds * 1000ULL);
#ifndef UNIT_TEST
  while (cSysTick::Micros() <= endTimeInMicros)
  {
  }
#endif
}

void cSysTick::DelayUs(ui64 microseconds)
{
  const ui64 endTimeInMicros = cSysTick::Micros() + microseconds;
#ifndef UNIT_TEST
  while (cSysTick::Micros() <= endTimeInMicros)
  {
  }
#endif
}
