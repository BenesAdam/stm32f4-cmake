#ifndef CAN_SIGNAL_HPP
#define CAN_SIGNAL_HPP

#include "types.h"
#include "can_message.hpp"

class cCanSignal
{
public:
  cCanSignal(const cCanMessage *const arg_message, const ui8 arg_byteIndex, const ui8 arg_size);

  void FetchData(void);
  ui32 Get(void);

private:
  const cCanMessage* message;
  ui8 byteIndex;
  ui8 size;
  ui32 data;
};

#endif /* CAN_SIGNAL_HPP */
