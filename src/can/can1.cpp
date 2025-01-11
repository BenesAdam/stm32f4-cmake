#include <cstring>

extern "C"
{
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
}

#include "can1.hpp"
#include "can_message.hpp"

cCan1::cCan1(void)
{
}

void cCan1::Setup(void)
{
  /* Rx PA11 */
  /* Tx PA12 */

  /* Enable peripherals */
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_CAN1);

  AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTA;

  /* Configure pins */
  /* - Rx - input pull-up */
  gpio_set_mode(GPIO_BANK_CAN1_RX, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_RX);
  gpio_set(GPIO_BANK_CAN1_RX, GPIO_CAN1_RX);

  /* - Tx - output AF pushpull */
  gpio_set_mode(GPIO_BANK_CAN1_TX, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_TX);

  /* NVIC */
  nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
  nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

  /* Reset CAN */
  can_reset(CAN1);

  /* Initialize CAN for 500 kBaud */
  bool initFailed = can_init(
      /* canport  */ CAN1,
      /* ttcm     */ false,
      /* abom     */ true,
      /* awum     */ true,
      /* nart     */ true,
      /* rflm     */ true,
      /* txfp     */ true,
      /* sjw      */ CAN_BTR_SJW_1TQ,
      /* ts1      */ CAN_BTR_TS1_15TQ,
      /* ts2      */ CAN_BTR_TS2_2TQ,
      /* brp      */ 4,
      /* loopback */ false,
      /* silent   */ false);

  if (initFailed)
  {
    debug();
  }

  /* CAN filter 0 init. */
  can_filter_id_mask_32bit_init(
      0,     /* Filter ID */
      0,     /* CAN ID */
      0,     /* CAN ID mask */
      0,     /* FIFO assignment (here: FIFO0) */
      true); /* Enable the filter. */

  /* Enable CAN RX interrupt. */
  can_enable_irq(CAN1, CAN_IER_FMPIE0);
}

extern "C" void usb_lp_can_rx0_isr(void)
{
  cCanMessage::sObjectReceived object;

  can_receive(CAN1, 0, false, &object.Identifier, &object.Extended, &object.RemoteTransitionRequest,
              &object.MatchedFilterIdentifier, &object.Length, object.Data, NULL);

  cCanMessage::IrqReceive(object);

  can_fifo_release(CAN1, 0);
}
