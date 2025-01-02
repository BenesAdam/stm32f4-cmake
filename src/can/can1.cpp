#include <cstring>

extern "C"
{
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
}

#include "can1.hpp"

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
  bool initDone = can_init(
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
    /* silent   */ false
  );

  // TODO: what if initialization is not done correctly?
}

void usb_lp_can_rx0_isr(void)
{
	uint32_t id;
	bool ext, rtr;
	uint8_t fmi, length, data[8];

	can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &length, data, NULL);

  // TODO: Do something?

	can_fifo_release(CAN1, 0);
}

void cCan1::TransmitNumber(const ui32 arg_number)
{
  /* Manual: 
  https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/obldc-strip/can/can.c 
  */

  ui8 data[8U];
  memset(data, 0U, 8U);
  memcpy(data, &arg_number, sizeof(ui32));

  can_transmit(CAN1, 0xAAU, false, false, sizeof(ui32), data);
}
