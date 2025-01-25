#include <cstring>
#include "can_message.hpp"
#include "can1.hpp"

cCanMessage::cCanMessage(const eDirection arg_direction, const eFormat arg_format, const ui32 arg_identifier, const ui8 arg_length)
    : direction(arg_direction),
      format(arg_format),
      identifier(arg_identifier),
      length(arg_length),
      data{0},
      received(false)
{
  sMessageList &messageList = (direction == eDirection::Receive) ? RxMessageList : TxMessageList;
  const bool dataStartIsNull = messageList.Messages == nullptr;
  const bool bufferExceed = messageList.Size >= messageList.MaxSize;

  if (dataStartIsNull || bufferExceed)
  {
    // You need to call CREATE_RX_MESSAGE_LIST(n) and CREATE_TX_MESSAGE_LIST(m)
    // somewhere in cpp file to define CAN message lists.
    debug();
    return;
  }

  messageList.Messages[messageList.Size] = this;
  messageList.Size++;
}

void cCanMessage::SetMessageList(const cCanMessage::eDirection arg_direction, cCanMessage *arg_messageList[], const ui16 arg_size)
{
  if (arg_direction == eDirection::Receive)
  {
    RxMessageList.Messages = arg_messageList;
    RxMessageList.MaxSize = arg_size;
  }
  else
  {
    TxMessageList.Messages = arg_messageList;
    TxMessageList.MaxSize = arg_size;
  }
}

void cCanMessage::Transmit(void)
{
  if (direction != eDirection::Transmit)
  {
    return;
  }

  can_transmit(CAN1, identifier, (format == eFormat::Extended), false, length, data);
}

void cCanMessage::IrqReceive(const sObjectReceived &arg_obj)
{
  const eFormat receivedFormat = (arg_obj.Extended) ? eFormat::Extended : eFormat::Standard;

  for (ui16 i = 0; i < RxMessageList.Size; ++i)
  {
    cCanMessage &message = *(RxMessageList.Messages[i]);

    if ((message.format == receivedFormat) && (message.identifier == arg_obj.Identifier))
    {
      message.CopyData(arg_obj);
      message.received = true;
      return;
    }
  }
}

void cCanMessage::CopyData(const sObjectReceived &arg_obj)
{
  for (ui8 i = 0U; (i < length) && (i < arg_obj.Length); i++)
  {
    data[i] = arg_obj.Data[i];
  }
}

ui32 cCanMessage::GetIdentifier(void) const
{
  return identifier;
}

void cCanMessage::GetData(const ui8 arg_byteIndex, const ui8 arg_size, ui8* arg_data) const
{
  if ((arg_byteIndex >= dataSize) || ((arg_byteIndex + arg_size) > dataSize))
  {
    return;
  }

  memcpy(arg_data, data + arg_byteIndex, arg_size);
}

void cCanMessage::SetData(const ui8 arg_byteIndex, const ui8 arg_size, ui8* arg_data)
{
  if ((arg_byteIndex >= dataSize) || ((arg_byteIndex + arg_size) > dataSize))
  {
    return;
  }

  memcpy(data + arg_byteIndex, arg_data, arg_size);
}
