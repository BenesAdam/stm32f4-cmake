#include "can_cfg.hpp"

sCanMessageList<5U> TxMessageList(cCanMessage::eDirection::Transmit);
sCanMessageList<5U> RxMessageList(cCanMessage::eDirection::Receive);

cCanMessage Rx_MsgTimeStamp(
  cCanMessage::eDirection::Receive,
  cCanMessage::eFormat::Standard,
  0xAA,
  4U
);

// cCanSignal Rx_TimeSeconds(
//   &Rx_MsgTimeStamp,
//   0U,
//   4U
// );

