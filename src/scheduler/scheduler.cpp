#include "scheduler.hpp"
#include "systick.hpp"
#include "errorhandler_inst.hpp"

cScheduler::cScheduler(
    const ui64 arg_frequenceMs,
    const sTimeSlot *const arg_timeSlots,
    const ui8 arg_length) : frequenceMicros(arg_frequenceMs * 1000ULL),
                            timeSlots(arg_timeSlots),
                            length(arg_length),
                            nextCycleStartMicros(0U),
                            cycleCounter(0U)
{
}

void cScheduler::Cyclic(void)
{
  ui64 timeStart;

  // Active waiting for next cycle
  do
  {
    timeStart = cSysTick::Micros();
  } while (timeStart <= nextCycleStartMicros);

  // Next cycle start
  nextCycleStartMicros = timeStart + frequenceMicros;

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

  // Check cycle runtime
  ui64 timeEnd = cSysTick::Micros();
  ui64 currentRuntime = timeEnd - timeStart;
  maxRuntime = (currentRuntime > maxRuntime) ? currentRuntime : maxRuntime;

  if (currentRuntime > frequenceMicros)
  {
    ErrorHandler.SetErrorActive(E_SCHEDULER_RUNTIME_EXCEED);
    debug();
  }
}
