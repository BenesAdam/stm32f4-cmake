#pragma once

#include "can_message.hpp"
#include "can_signal.hpp"

extern cCanMessage Rx_MsgTimeStamp;
extern cCanSignal Rx_TimeSeconds;

extern cCanMessage Tx_MsgUniqueIdentifier0;
extern cCanMessage Tx_MsgUniqueIdentifier1;
extern cCanSignal Tx_UniqueIdentifier0;
extern cCanSignal Tx_UniqueIdentifier1;
extern cCanSignal Tx_UniqueIdentifier2;