extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

#include "systick.hpp"
#include "i2c1.hpp"
#include "display.hpp"
#include "noinit_vars.hpp"
#include "errorhandler_inst.hpp"
#include "can1.hpp"

#define LED_PORT GPIOC
#define LED_PIN GPIO13

void InitializeLight(void);
void BlinkShort(void);
void Toggle(void);

void SendTime(void);

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();
  cCan1::Setup();

  InitializeLight();

  while (true)
  {
    ErrorHandler.Cyclic();

    SendTime();
    cSysTick::DelayMs(100U);
  }

  return 0;
}

void InitializeLight(void)
{
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}

void BlinkShort(void)
{
  static const ui64 shortBlinkMs = 200U;

  gpio_clear(LED_PORT, LED_PIN);
  cSysTick::DelayMs(shortBlinkMs);
  gpio_set(LED_PORT, LED_PIN);
  cSysTick::DelayMs(shortBlinkMs);
}

void Toggle(void)
{
  gpio_toggle(LED_PORT, LED_PIN);
}

void SendTime(void)
{
  static ui64 startMeasuringTime = cSysTick::Millis();
  static ui64 nextToggleTime = 0U;

  ui64 currentTime = cSysTick::Millis() - startMeasuringTime;
  ui32 seconds = currentTime / 1000ULL;
  ui32 millis = currentTime % 1000ULL;

  if (currentTime >= nextToggleTime)
  {
    gpio_toggle(LED_PORT, LED_PIN);

    // Send time via CAN
    cCan1::TransmitNumber(seconds);

    nextToggleTime = currentTime + 1000ULL;
  }
}
