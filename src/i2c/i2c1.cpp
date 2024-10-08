#include "i2c1.hpp"
#include "systick.cpp"

extern "C"
{
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
}

#define I2C_TIMEOUT_DECLARE(arg_startTime) ui64 arg_startTime
#define I2C_TIMEOUT_START(arg_startTime) arg_startTime = cSysTick::Micros()
#define I2C_TIMEOUT_CHECK(arg_startTime)                     \
  if (cSysTick::Micros() - arg_startTime > cI2C1::timeoutUs) \
  {                                                          \
    /* TODO: Add error to error memory */                    \
    return;                                                  \
  }

void cI2C1::Setup(void)
{
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_I2C1);

  // Set GPIO port B
  const ui16 gpioPins = GPIO8 | GPIO9;

#ifdef STM32F4
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, gpioPins);
  gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, gpioPins);
  gpio_set_af(GPIOB, GPIO_AF4, gpioPins);
#else
  rcc_periph_clock_enable(RCC_AFIO);
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_I2C1_REMAP); // map I2C1 from PB6/7 to PB8/9
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, gpioPins);
#endif

  // Set I2C1
  i2c_peripheral_disable(I2C1);
  i2c_set_speed(I2C1, i2c_speed_sm_100k, rcc_apb1_frequency / 1e6);
  i2c_peripheral_enable(I2C1);
}

void cI2C1::WriteByte(const ui8 arg_address, const ui8 arg_data)
{
  Transfer7(I2C1, arg_address, &arg_data, 1U, nullptr, 0U);
}

ui8 cI2C1::ReadByte(const ui8 arg_address)
{
  ui8 result = 0U;
  Transfer7(I2C1, arg_address, nullptr, 0U, &result, 1U);
  return result;
}

ui8 cI2C1::WriteReadByte(const ui8 arg_address, const ui8 arg_data)
{
  ui8 result = 0U;
  Transfer7(I2C1, arg_address, &arg_data, 1U, &result, 1U);
  return result;
}

/**
 * Run a write/read transaction to a given 7bit i2c address
 * If both write & read are provided, the read will use repeated start.
 * Both write and read are optional
 * There are likely still issues with repeated start/stop condtions!
 * @param i2c peripheral of choice, eg I2C1
 * @param addr 7 bit i2c device address
 * @param w buffer of data to write
 * @param wn length of w
 * @param r destination buffer to read into
 * @param rn number of bytes to read (r should be at least this long)
 */
void cI2C1::Transfer7(ui32 i2c, ui8 addr, const ui8 *w, size_t wn, ui8 *r, size_t rn)
{
  // Redefinition of i2c_transfer7
  if (wn)
  {
    Write7(i2c, addr, w, wn);
  }

  if (rn)
  {
    Read7(i2c, addr, r, rn);
  }

  i2c_send_stop(i2c);
}

/*
 * Redefinition of i2c_write7_v1
 */
void cI2C1::Write7(ui32 i2c, ui8 addr, const ui8 *data, size_t n)
{
  I2C_TIMEOUT_DECLARE(timer);

  I2C_TIMEOUT_START(timer);
  while ((I2C_SR2(i2c) & I2C_SR2_BUSY))
  {
    I2C_TIMEOUT_CHECK(timer);
  }

  i2c_send_start(i2c);

  /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
  I2C_TIMEOUT_START(timer);
  while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & I2C_SR2_MSL) && (I2C_SR2(i2c) & I2C_SR2_BUSY)))
  {
    I2C_TIMEOUT_CHECK(timer);
  }

  i2c_send_7bit_address(i2c, addr, I2C_WRITE);

  /* Waiting for address is transferred. */
  I2C_TIMEOUT_START(timer);
  while (!(I2C_SR1(i2c) & I2C_SR1_ADDR))
  {
    I2C_TIMEOUT_CHECK(timer);
  }

  /* Clearing ADDR condition sequence. */
  (void)I2C_SR2(i2c);

  for (size_t i = 0; i < n; i++)
  {
    I2C_TIMEOUT_START(timer);
    i2c_send_data(i2c, data[i]);
    while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)))
    {
      I2C_TIMEOUT_CHECK(timer);
    }
  }
}

/*
 * Redefinition of i2c_read7_v1
 */
void cI2C1::Read7(ui32 i2c, ui8 addr, ui8 *res, size_t n)
{
  I2C_TIMEOUT_DECLARE(timer);
  i2c_send_start(i2c);
  i2c_enable_ack(i2c);

  /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
  I2C_TIMEOUT_START(timer);
  while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & I2C_SR2_MSL) && (I2C_SR2(i2c) & I2C_SR2_BUSY)))
  {
    I2C_TIMEOUT_CHECK(timer);
  }

  i2c_send_7bit_address(i2c, addr, I2C_READ);

  /* Waiting for address is transferred. */
  I2C_TIMEOUT_START(timer);
  while (!(I2C_SR1(i2c) & I2C_SR1_ADDR))
  {
    I2C_TIMEOUT_CHECK(timer);
  }

  /* Clearing ADDR condition sequence. */
  (void)I2C_SR2(i2c);

  for (size_t i = 0; i < n; ++i)
  {
    if (i == n - 1)
    {
      i2c_disable_ack(i2c);
    }

    I2C_TIMEOUT_START(timer);
    while (!(I2C_SR1(i2c) & I2C_SR1_RxNE))
    {
      I2C_TIMEOUT_CHECK(timer);
    }

    res[i] = i2c_get_data(i2c);
  }
}
