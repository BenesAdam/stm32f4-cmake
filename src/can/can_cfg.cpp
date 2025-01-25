#include "can_cfg.hpp"

CREATE_RX_MESSAGE_LIST(4U);
CREATE_TX_MESSAGE_LIST(5U);

/* Rx Counter*/
cCanMessage Rx_MsgCounter(
    cCanMessage::eDirection::Receive,
    cCanMessage::eFormat::Standard,
    BASE_RX_CAN_ID + 0U,
    4U);

cCanSignal Rx_CounterInvertUi16(
    &Rx_MsgCounter,
    0U,
    2U);

cCanSignal Rx_CounterUi16(
    &Rx_MsgCounter,
    2U,
    2U);

/* Rx unique identifier */
cCanMessage Rx_MsgUniqueIdentifier0(
    cCanMessage::eDirection::Receive,
    cCanMessage::eFormat::Standard,
    BASE_RX_CAN_ID + 1U,
    8U);

cCanSignal Rx_UniqueIdentifier0(
    &Rx_MsgUniqueIdentifier0,
    0U,
    4U);

cCanSignal Rx_UniqueIdentifier1(
    &Rx_MsgUniqueIdentifier0,
    4U,
    4U);

/* Tx Counter */
cCanMessage Tx_MsgCounter(
    cCanMessage::eDirection::Transmit,
    cCanMessage::eFormat::Standard,
    BASE_TX_CAN_ID + 0U,
    4U);

cCanSignal Tx_CounterInvertUi16(
    &Tx_MsgCounter,
    0U,
    2U);

cCanSignal Tx_CounterUi16(
    &Tx_MsgCounter,
    2U,
    2U);

/* Tx unique identifier */
cCanMessage Tx_MsgUniqueIdentifier0(
    cCanMessage::eDirection::Transmit,
    cCanMessage::eFormat::Standard,
    BASE_TX_CAN_ID + 1U,
    8U);

cCanSignal Tx_UniqueIdentifier0(
    &Tx_MsgUniqueIdentifier0,
    0U,
    4U);

cCanSignal Tx_UniqueIdentifier1(
    &Tx_MsgUniqueIdentifier0,
    4U,
    4U);
