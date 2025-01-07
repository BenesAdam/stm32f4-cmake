#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "types.h"

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
  const ui64 frequenceMicros;
  const sTimeSlot *const timeSlots;
  const ui8 length;

  ui64 nextCycleStartMicros;
  ui32 cycleCounter;
  ui64 maxRuntime;
};

#endif /* SCHEDULER_HPP */
