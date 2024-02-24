#include "prj_systick.hpp"
#include "libopencm3/cm3/systick.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/nvic.h"

static volatile uint64_t system_time = 0U;

extern "C" void sys_tick_handler(void)
{
  system_time++;
}

void cSysTick::Setup(void)
{
  systick_set_frequency(cSysTick::Frequency, rcc_ahb_frequency);
  systick_counter_enable();
  systick_interrupt_enable();
}

// Returns number of tenths of milliseconds (1 tick == 0.1ms)
uint64_t cSysTick::Get(void)
{
  return system_time;
}

uint64_t cSysTick::Millis(void)
{
  return cSysTick::Get() / 10ULL;
}

void cSysTick::Wait(uint64_t ms)
{
  const uint64_t endTimeInTenthOfMs = cSysTick::Get() + (ms * 10ULL);
  while(cSysTick::Get() < endTimeInTenthOfMs)
  {
  }
}
