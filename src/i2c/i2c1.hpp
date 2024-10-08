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
  static void Transfer7(ui32 i2c, ui8 addr, const ui8 *w, size_t wn, ui8 *r, size_t rn);
  static void Write7(ui32 i2c, ui8 addr, const ui8 *data, size_t n);
  static void Read7(ui32 i2c, ui8 addr, ui8 *res, size_t n);

  static const ui64 timeoutUs = 300ULL;
};

#endif /* I2C1_HPP */
