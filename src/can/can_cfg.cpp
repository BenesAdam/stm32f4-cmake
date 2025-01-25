#include "can_cfg.hpp"

CREATE_RX_MESSAGE_LIST(4U);
CREATE_TX_MESSAGE_LIST(5U);

cCanMessage Rx_MsgTimeStamp(
    cCanMessage::eDirection::Receive,
    cCanMessage::eFormat::Standard,
    0xAA,
    4U);

cCanSignal Rx_TimeSeconds(
  &Rx_MsgTimeStamp,
  0U,
  4U
);

cCanMessage Tx_MsgUniqueIdentifier0(
  cCanMessage::eDirection::Transmit,
  cCanMessage::eFormat::Standard,
  0xA0,
  8U
);

cCanMessage Tx_MsgUniqueIdentifier1(
  cCanMessage::eDirection::Transmit,
  cCanMessage::eFormat::Standard,
  0xA1,
  4U
);

cCanSignal Tx_UniqueIdentifier0(
  &Tx_MsgUniqueIdentifier0,
  0U,
  4U
);

cCanSignal Tx_UniqueIdentifier1(
  &Tx_MsgUniqueIdentifier0,
  4U,
  4U
);

cCanSignal Tx_UniqueIdentifier2(
  &Tx_MsgUniqueIdentifier1,
  0U,
  4U
);
