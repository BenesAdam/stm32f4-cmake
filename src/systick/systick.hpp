#ifndef systick_HPP
#define systick_HPP

#include "types.h"

class cSysTick
{
public:
  static const ui32 Frequency = 1000000U; // 1MHz
  static void Setup(void);
  static ui64 Millis(void);
  static ui64 Micros(void);
  static void DelayMs(ui64 milliseconds);
  static void DelayUs(ui64 microseconds);
};

#endif /* systick_HPP */
