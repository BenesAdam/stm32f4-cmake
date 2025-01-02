#ifndef CAN1_HPP
#define CAN1_HPP

#include "types.h"

extern "C"
{
  #include "libopencm3/stm32/can.h"
}

class cCan1
{
public:
  cCan1(void);
  static void Setup(void);
  static void TransmitNumber(const ui32 arg_number);

private:
};

#endif /* CAN1_HPP */
