#include "can_message.hpp"
#include "can1.hpp"

cCanMessage::sMessageList cCanMessage::RxMessageList;
cCanMessage::sMessageList cCanMessage::TxMessageList;

volatile ui32 Global_MsgCtor_Good = 0U;
volatile ui32 Global_MsgCtor_Addr = 0U;
volatile ui32 Global_MsgCtor_DataAddr = 0U;
volatile ui32 Gloval_MsgCtor_InArray = 0U;
volatile ui32 Gloval_Ctor_CallOrder = 0U;

cCanMessage::cCanMessage(const eDirection arg_direction, const eFormat arg_format, const ui32 arg_identifier, const ui8 arg_length)
    : direction(arg_direction),
      format(arg_format),
      identifier(arg_identifier),
      length(arg_length),
      data{0}
{
  sMessageList& messageList = (direction == eDirection::Receive) ? RxMessageList : TxMessageList;

  // TODO: move this to message list structure
  const bool dataStartIsNull = messageList.DataStart == nullptr;
  const bool bufferExceed = messageList.Size >= messageList.MaxSize;

  if (dataStartIsNull || bufferExceed)
  {
    // TODO: Check message list
    debug();
    return;
  }


  messageList.DataStart[messageList.Size] = this;
  
  
  Global_MsgCtor_Good++;
  Global_MsgCtor_Addr = (ui32)this;
  Gloval_MsgCtor_InArray = (ui32)messageList.DataStart[0];



  messageList.Size++;

  Gloval_Ctor_CallOrder = (Gloval_Ctor_CallOrder << 8) | 0x01;
}

void cCanMessage::SetMessageList(const cCanMessage::eDirection arg_direction, cCanMessage **arg_messageList, const ui16 arg_size)
{
  if(arg_direction == eDirection::Receive)
  {
    Global_messagesAddr = (ui32)arg_messageList;
    RxMessageList.DataStart = arg_messageList;
    RxMessageList.MaxSize = arg_size;

    Global_MsgCtor_DataAddr = (ui32)RxMessageList.DataStart;

    Gloval_Ctor_CallOrder = (Gloval_Ctor_CallOrder << 8) | 0x02;
  }
  else
  {
    TxMessageList.DataStart = arg_messageList;
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

void cCanMessage::Receive(void)
{
  volatile cCanMessage* message = RxMessageList.DataStart[0];

  volatile auto id = message->identifier;
  volatile auto size = message->dataSize;
}

ui32 cCanMessage::GetIdentifier(void) const
{
  return identifier;
}

cCanMessage::sMessageList::sMessageList(void)
: DataStart(nullptr),
  MaxSize(0U),
  Size(0U)
{
}
