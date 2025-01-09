#pragma once

#include "types.h"

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
    cCanMessage** DataStart;
    ui16 MaxSize;
    ui16 Size;

    sMessageList(void);
  };

  cCanMessage(const eDirection arg_direction, const eFormat arg_format, const ui32 arg_identifier, const ui8 arg_length);
  static void SetMessageList(const cCanMessage::eDirection arg_direction, cCanMessage** arg_messageList, const ui16 arg_size);
  
  void Transmit(void);
  static void Receive(void);

  ui32 GetIdentifier(void) const;

  static sMessageList RxMessageList;
  static sMessageList TxMessageList;

private:
  const eDirection direction;
  const eFormat format;
  const ui32 identifier;
  const ui8 length;

  static const ui8 dataSize = 8U;
  ui8 data[dataSize]; // TODO: make data size based on constructor parameter length
};

template <ui16 Size>
struct sCanMessageList
{
  cCanMessage *messages[Size];
  const ui16 size = Size;
 
  explicit sCanMessageList(const cCanMessage::eDirection arg_direction);
};

extern volatile ui32 Global_messagesAddr;

template <ui16 Size>
inline sCanMessageList<Size>::sCanMessageList(const cCanMessage::eDirection arg_direction)
{
  cCanMessage::SetMessageList(arg_direction, this->messages, this->size);
}
