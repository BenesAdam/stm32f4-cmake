#pragma once

#include "can_message.hpp"
#include "can_signal.hpp"

#define CAN_BASE_ID_1 0xA0U
#define CAN_BASE_ID_2 0xB0U

#if ECU_NEW == 1
  #define BASE_RX_CAN_ID CAN_BASE_ID_1
  #define BASE_TX_CAN_ID CAN_BASE_ID_2
#else
  #define BASE_RX_CAN_ID CAN_BASE_ID_2
  #define BASE_TX_CAN_ID CAN_BASE_ID_1
#endif

extern cCanMessage Rx_MsgCounter;
extern cCanSignal Rx_CounterUi16;
extern cCanSignal Rx_CounterInvertUi16;
extern cCanMessage Rx_MsgUniqueIdentifier0;
extern cCanSignal Rx_UniqueIdentifier0;
extern cCanSignal Rx_UniqueIdentifier1;

extern cCanMessage Tx_MsgCounter;
extern cCanSignal Tx_CounterUi16;
extern cCanSignal Tx_CounterInvertUi16;
extern cCanMessage Tx_MsgUniqueIdentifier0;
extern cCanSignal Tx_UniqueIdentifier0;
extern cCanSignal Tx_UniqueIdentifier1;
