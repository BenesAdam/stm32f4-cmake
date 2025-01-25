#include "can_signal.hpp"

cCanSignal::cCanSignal(cCanMessage *const arg_message, const ui8 arg_byteIndex, const ui8 arg_size) :
  message(arg_message),
  byteIndex(arg_byteIndex),
  size(arg_size)
{
}

void cCanSignal::FetchData(void)
{
  data = 0U;
  message->GetData(byteIndex, size, reinterpret_cast<ui8*>(&data));
}

void cCanSignal::StoreData(const ui32 arg_data)
{
  data = arg_data;
  message->SetData(byteIndex, size, reinterpret_cast<ui8*>(&data));
}

ui32 cCanSignal::Get(void)
{
  return data;
}
