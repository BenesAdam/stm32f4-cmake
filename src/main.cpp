#include "prj_systick.hpp"

extern "C" 
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

#define LED_PORT GPIOC
#define RCC_LED_PORT RCC_GPIOC
#define LED_PIN GPIO13

int main() {
  cSysTick::Setup();

  rcc_periph_clock_enable(RCC_LED_PORT);
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

  while(true)
  {
    gpio_set(LED_PORT, LED_PIN);
    cSysTick::Wait(100);
    gpio_clear(LED_PORT, LED_PIN);
    cSysTick::Wait(4900);
  }
}
