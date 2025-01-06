#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "types.h"

extern "C"
{
  // #include "libopencm3/stm32/i2c.h"
}

class cScheduler
{
public:
  typedef void (*tTask)(void);

  struct sTimeSlot
  {
    ui32 time;
    tTask task;
  };

  cScheduler(const ui64 arg_frequenceMs, const sTimeSlot *const arg_timeSlots, const ui8 arg_length);

  void Cyclic(void);

private:
  const ui64 frequenceMs;
  const sTimeSlot *const timeSlots;
  const ui8 length;

  ui64 nextCycleStartMs;
  ui32 cycleCounter;
};

#endif /* SCHEDULER_HPP */
