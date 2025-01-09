#include "can_cfg.hpp"

CREATE_RX_MESSAGE_LIST(4U);
CREATE_TX_MESSAGE_LIST(5U);

cCanMessage Rx_MsgTimeStamp(
    cCanMessage::eDirection::Receive,
    cCanMessage::eFormat::Standard,
    0xAA,
    4U);

// cCanSignal Rx_TimeSeconds(
//   &Rx_MsgTimeStamp,
//   0U,
//   4U
// );
