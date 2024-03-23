#ifndef I2C1_HPP
#define I2C1_HPP

#include "types.h"

extern "C"
{
#include "libopencm3/stm32/i2c.h"
}

class cI2C1
{
public:
  enum eOperation
  {
    E_READ = I2C_READ,
    E_WRITE = I2C_WRITE
  };

  static void Setup(void);

private:
};

#endif /* I2C1_HPP */
