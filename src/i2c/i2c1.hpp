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
  static void WriteByte(const ui8 arg_address, const ui8 arg_data);
  static ui8 ReadByte(const ui8 arg_address);
  static ui8 WriteReadByte(const ui8 arg_address, const ui8 arg_data);

private:
};

#endif /* I2C1_HPP */
