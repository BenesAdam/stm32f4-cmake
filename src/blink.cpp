#include "prj_systick.hpp"

extern "C" {
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

#define LED_PORT GPIOA
#define RCC_LED_PORT RCC_GPIOA
#define LED_PIN GPIO11

int main() {
  cSysTick::Setup();

  rcc_periph_clock_enable(RCC_LED_PORT);
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
  
  while(true)
  {
    cSysTick::Wait(5000);
    gpio_toggle(LED_PORT, LED_PIN);
  }
}
