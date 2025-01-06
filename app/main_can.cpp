extern "C"
{
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
}

#include "can1.hpp"
#include "errorhandler_inst.hpp"
#include "noinit_vars.hpp"
#include "scheduler.hpp"
#include "systick.hpp"

#define LED_PORT GPIOC
#define LED_PIN GPIO13

void InitializeLight(void);

void Task100ms(void);
void Task1000ms(void);

const cScheduler::sTimeSlot TimeSlots[] =
{
  { 1U, &Task100ms},
  {10U, &Task1000ms}
};
const ui8 TimeSlotsLen = sizeof(TimeSlots) / sizeof(TimeSlots[0U]);
cScheduler Scheduler(100U, &TimeSlots[0U], TimeSlotsLen);

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();
  cCan1::Setup();

  InitializeLight();

  while (true)
  {
    Scheduler.Cyclic();
  }

  return 0;
}

void InitializeLight(void)
{
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}

void Task100ms(void)
{
  ErrorHandler.Cyclic();

  ui32 seconds = cSysTick::Millis() / 1000ULL;
  cCan1::TransmitNumber(seconds);
}

void Task1000ms(void)
{
  gpio_toggle(LED_PORT, LED_PIN);
}
