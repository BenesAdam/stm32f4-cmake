#include "i2c1.hpp"

extern "C"
{
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
}

// TODO: GPIO port and pins as parameter
// TODO: Compute RCC perith clock enable
void cI2C1::Setup(void)
{
  const ui16 gpioPins = GPIO8 | GPIO9;

  // Set GPIO port B
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, gpioPins);
  gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, gpioPins);
  gpio_set_af(GPIOB, GPIO_AF4, gpioPins);

  // Set I2C1
  rcc_periph_clock_enable(RCC_I2C1);

  i2c_peripheral_disable(I2C1);
  i2c_set_speed(I2C1, i2c_speed_sm_100k, rcc_apb1_frequency / 1e6);

  i2c_peripheral_enable(I2C1);
}

void cI2C1::WriteByte(const ui8 arg_address, const ui8 arg_data)
{
  i2c_transfer7(I2C1, arg_address, &arg_data, 1U, nullptr, 0U);
}

ui8 cI2C1::ReadByte(const ui8 arg_address)
{
  ui8 result = 0U;
  i2c_transfer7(I2C1, arg_address, nullptr, 0U, &result, 1U);
  return result;
}

ui8 cI2C1::WriteReadByte(const ui8 arg_address, const ui8 arg_data)
{
  ui8 result = 0U;
  i2c_transfer7(I2C1, arg_address, &arg_data, 1U, &result, 1U);
  return result;
}
