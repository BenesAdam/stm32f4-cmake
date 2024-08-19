#ifndef systick_HPP
#define systick_HPP

#include "types.h"

class cSysTick
{
public:
  static const ui32 Frequency = 1000000U; // 1MHz
  static void Setup(void);
  static uint64_t Millis(void);
  static uint64_t Micros(void);
  static void DelayMs(uint64_t milliseconds);
  static void DelayUs(uint64_t microseconds);
};

#endif /* systick_HPP */
