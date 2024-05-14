#include "systick.hpp"

extern "C"
{
#include "libopencm3/cm3/systick.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/nvic.h"  
}

static volatile uint64_t system_time = 0U;

extern "C" void sys_tick_handler(void)
{
  system_time++;
}

void cSysTick::Setup(void)
{
  // Configure general clock
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);

  // Configure system timer
  systick_set_frequency(cSysTick::Frequency, rcc_ahb_frequency);
  systick_counter_enable();
  systick_interrupt_enable();
}

// Returns number of microseconds
uint64_t cSysTick::Micros(void)
{
  return system_time;
}

uint64_t cSysTick::Millis(void)
{
  return cSysTick::Micros() / 1000ULL;
}

void cSysTick::DelayMs(uint64_t milliseconds)
{
  const uint64_t endTimeInMicros = cSysTick::Micros() + (milliseconds * 1000ULL);
  while(cSysTick::Micros() <= endTimeInMicros)
  {
  }
}

void cSysTick::DelayUs(uint64_t microseconds)
{
  const uint64_t endTimeInMicros = cSysTick::Micros() + microseconds;
  while(cSysTick::Micros() <= endTimeInMicros)
  {
  }
}
