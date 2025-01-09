extern "C"
{
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
}

#include "can1.hpp"
#include "errorhandler_inst.hpp"
#include "noinit_vars.hpp"
#include "scheduler.hpp"
#include "systick.hpp"
#include "can_cfg.hpp"

#define LED_PORT GPIOC
#define LED_PIN GPIO13

void InitializeLight(void);

void Task100ms(void);
void Task1000ms(void);

const cScheduler::sTimeSlot TimeSlots[] =
    {
        {1U, &Task100ms},
        {10U, &Task1000ms}};
const ui8 TimeSlotsLen = sizeof(TimeSlots) / sizeof(TimeSlots[0U]);
cScheduler Scheduler(100U, &TimeSlots[0U], TimeSlotsLen);

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();
  cCan1::Setup();

  InitializeLight();

  cSysTick::DelayMs(2000U);

  while (true)
  {
    Scheduler.Cyclic();
  }

  return 0;
}

void InitializeLight(void)
{
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}

// ECU New
#define ECU_NEW 0

#if (ECU_NEW == 1)
// Send time in seconds every 100ms
// Do nothing with received messages
void Task100ms(void)
{
  ErrorHandler.Cyclic();

  ui32 seconds = cSysTick::Millis() / 1000ULL;
  ui8 data[8U];
  *data = seconds; // dirty but works
  can_transmit(CAN1, 0xAAU, false, false, sizeof(ui32), data);
}
#else
// Receive time from other message
// Transmit time + 0xAA000000
volatile ui32 Global_messagesAddr;

void Task100ms(void)
{
  ErrorHandler.Cyclic();

  // ui32 seconds = cSysTick::Millis() / 1000ULL;
  // cCan1::TransmitNumber(seconds);

  // volatile ui32 MessageAddr = (ui32)&Rx_MsgTimeStamp;
  // volatile auto time = Rx_MsgTimeStamp.GetIdentifier();

  // volatile ui32 RxBufferAddr = (ui32)&RxMessageList;
  // volatile ui32 RxBufferDataStartAddr = (ui32)&RxMessageList.messages;
  // volatile ui32 StaticCanMessageDataStartAddr = (ui32)cCanMessage::RxMessageList.DataStart;
  // volatile ui32 StaticCanMessageMessageAddr = (ui32)cCanMessage::RxMessageList.DataStart[0];

  // cCanMessage::Receive();

  volatile ui8 a = 5;
  sCanMessageList<2U> loc_RxMessageList(cCanMessage::eDirection::Receive);
  volatile ui32 addr_list = (ui32)&loc_RxMessageList;

  cCanMessage Rx_Msg00(cCanMessage::eDirection::Receive, cCanMessage::eFormat::Standard, 50U, 8U);
  cCanMessage Rx_Msg01(cCanMessage::eDirection::Receive, cCanMessage::eFormat::Standard, 51U, 8U);

  cCanMessage::Receive();

}
#endif

void Task1000ms(void)
{
  gpio_toggle(LED_PORT, LED_PIN);
}
