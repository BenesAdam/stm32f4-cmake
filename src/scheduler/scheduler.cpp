#include "scheduler.hpp"
#include "systick.hpp"

cScheduler::cScheduler(
    const ui64 arg_frequenceMs,
    const sTimeSlot *const arg_timeSlots,
    const ui8 arg_length) : frequenceMs(arg_frequenceMs),
                            timeSlots(arg_timeSlots),
                            length(arg_length),
                            nextCycleStartMs(0U),
                            cycleCounter(0U)
{
}

void cScheduler::Cyclic(void)
{
  ui64 time;

  // Active waiting for next cycle
  do
  {
    time = cSysTick::Millis();
  } while (time < nextCycleStartMs);

  // Next cycle start
  nextCycleStartMs = time + frequenceMs;

  // Cycle counter (32bit number used for modulo which is much cheaper than ui64 modulo)
  cycleCounter++;

  // Run all time slots
  for (ui8 i = 0U; i < length; ++i)
  {
    const sTimeSlot &timeSlot = timeSlots[i];

    if ((cycleCounter % timeSlot.time) == 0U)
    {
      timeSlot.task();
    }
  }
}
