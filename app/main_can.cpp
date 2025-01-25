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
  gpio_clear(LED_PORT, LED_PIN);

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

enum class eState
{
  DETERMINE_LOWER_ID,
  COOLDOWN,
  ACTIVE,
  PASSIVE
};

void Task100ms(void)
{
  ErrorHandler.Cyclic();

  const ui16 waitTimeCycles = 20U; // 5 seconds
  static eState state = eState::DETERMINE_LOWER_ID;
  static ui16 waitCounter = 0U;
  static ui16 sendCounter = 0U;
  static bool winner = false;

  switch (state)
  {
  case eState::DETERMINE_LOWER_ID:
  {
    Tx_UniqueIdentifier0.StoreData(DESIG_UNIQUE_ID0);
    Tx_UniqueIdentifier1.StoreData(DESIG_UNIQUE_ID1);
    Tx_MsgUniqueIdentifier0.Transmit();

    if (Rx_MsgUniqueIdentifier0.IsReceived())
    {
      Rx_UniqueIdentifier0.FetchData();
      Rx_UniqueIdentifier1.FetchData();
      Rx_MsgUniqueIdentifier0.ResetReceived();

      const ui64 txUniqueIdentifier = (static_cast<ui64>(DESIG_UNIQUE_ID0) << 32U) | (DESIG_UNIQUE_ID1);
      const ui64 rxUniqueIdentifier = (static_cast<ui64>(Rx_UniqueIdentifier0.Get()) << 32U) | (Rx_UniqueIdentifier1.Get());
      winner = (txUniqueIdentifier > rxUniqueIdentifier);

      waitCounter = 2U;

      if (winner)
      {
        waitCounter *= 2U;
      }

      state = eState::COOLDOWN;
    }
  }
  break;

  case eState::COOLDOWN:
  {
    Tx_UniqueIdentifier0.StoreData(DESIG_UNIQUE_ID0);
    Tx_UniqueIdentifier1.StoreData(DESIG_UNIQUE_ID1);
    Tx_MsgUniqueIdentifier0.Transmit();

    if (winner)
    {
      gpio_clear(LED_PORT, LED_PIN);
    }
    else
    {
      gpio_set(LED_PORT, LED_PIN);
    }

    if (waitCounter == 0U)
    {
      state = (winner) ? eState::ACTIVE : eState::PASSIVE;
    }
    else
    {
      waitCounter--;
    }
  }
  break;

  case eState::ACTIVE:
  {
    gpio_clear(LED_PORT, LED_PIN);

    if (waitCounter == 0U)
    {
      Tx_CounterUi16.StoreData(sendCounter);
      Tx_CounterInvertUi16.StoreData(~sendCounter);
      Tx_MsgCounter.Transmit();

      state = eState::PASSIVE;
    }
    else
    {
      waitCounter--;
    }
  }
  break;

  case eState::PASSIVE:
  {
    gpio_set(LED_PORT, LED_PIN);

    if (Rx_MsgCounter.IsReceived())
    {
      Rx_CounterUi16.FetchData();
      Rx_MsgCounter.ResetReceived();

      sendCounter = Rx_CounterUi16.Get() + 1U;
      waitCounter = waitTimeCycles;
      state = eState::ACTIVE;
    }
  }
  break;

  default:
    break;
  }
}

void Task1000ms(void)
{
}
