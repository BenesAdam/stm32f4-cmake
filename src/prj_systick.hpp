#ifndef PRJ_SYSTICK_HPP
#define PRJ_SYSTICK_HPP

#include "common-defines.h"

class cSysTick
{
public:
  static const uint32_t Frequency = 10000U; // 10kHz (every 100us)
  static void Setup(void);
  static uint64_t Get(void);
  static uint64_t Millis(void);
  static void Wait(uint64_t ms);
};

#endif /* PRJ_SYSTICK_HPP */
