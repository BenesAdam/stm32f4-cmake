#include "systick.hpp"
#include "i2c1.hpp"

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

#define LED_PORT GPIOC
#define RCC_LED_PORT RCC_GPIOC
#define LED_PIN GPIO13

#define I2C_PORT GPIOB
#define RCC_I2C_PORT RCC_GPIOB
#define I2C_SCL1_PIN GPIO8
#define I2C_SDA1_PIN GPIO9

int main()
{
  cSysTick::Setup();
  cI2C1::Setup();

  // Display vychozi I2C adresa: 0x27
  uint8_t dataWrite = 0x01;
  //i2c_transfer7(I2C1, 0x27, &dataWrite, 1U, nullptr, 0U);

  rcc_periph_clock_enable(RCC_LED_PORT);
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

  while (true)
  {
    i2c_transfer7(I2C1, 0x27, &dataWrite, 1U, nullptr, 0U);
    gpio_set(LED_PORT, LED_PIN);
    cSysTick::Wait(300);
    gpio_clear(LED_PORT, LED_PIN);
    cSysTick::Wait(1900);
  }
}
