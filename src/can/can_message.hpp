#pragma once

#include "types.h"

#define CREATE_RX_MESSAGE_LIST(Size) _CREATE_CAN_MESSAGE_LIST(Rx, Size, Receive)
#define CREATE_TX_MESSAGE_LIST(Size) _CREATE_CAN_MESSAGE_LIST(Tx, Size, Transmit)

#define _CREATE_CAN_MESSAGE_LIST(Prefix, Size, Type)\
  cCanMessage::sMessageList cCanMessage::Prefix##MessageList;\
  namespace nsCanMessagePrivate\
  {\
    sCanMessageList<Size> Prefix##MessageList(cCanMessage::eDirection::Type);\
  }

// Important
// Use CREATE_RX_MESSAGE_LIST and CREATE_TX_MESSAGE_LIST in cpp file before
// defining any cCanMessage.
class cCanMessage
{
public:
  enum class eDirection
  {
    Transmit,
    Receive
  };

  enum class eFormat
  {
    Standard,
    Extended
  };

  struct sMessageList
  {
    cCanMessage** Messages;
    ui16 MaxSize;
    ui16 Size;
  };

  struct sObjectReceived
  {
    ui32 Identifier;
	  bool Extended;
    bool RemoteTransitionRequest;
    ui8  MatchedFilterIdentifier;
    ui8  Length;
    ui8  Data[8U];
  };

  cCanMessage(const eDirection arg_direction, const eFormat arg_format, const ui32 arg_identifier, const ui8 arg_length);
  static void SetMessageList(const cCanMessage::eDirection arg_direction, cCanMessage *arg_messageList[], const ui16 arg_size);
  
  void Transmit(void);
  static void IrqReceive(const sObjectReceived& arg_obj);

  ui32 GetIdentifier(void) const;

  static sMessageList RxMessageList;
  static sMessageList TxMessageList;

private:
  void CopyData(const sObjectReceived& arg_obj);

  const eDirection direction;
  const eFormat format;
  const ui32 identifier;
  const ui8 length;

  static const ui8 dataSize = 8U;
  ui8 data[dataSize]; // TODO: make data size based on constructor parameter length
  bool received;
};

template <ui16 Size>
struct sCanMessageList
{
  cCanMessage *messages[Size];
  const ui16 size = Size;
 
  sCanMessageList(const cCanMessage::eDirection arg_direction);
};

template <ui16 Size>
inline sCanMessageList<Size>::sCanMessageList(const cCanMessage::eDirection arg_direction)
{
  cCanMessage::SetMessageList(arg_direction, this->messages, this->size);
}
