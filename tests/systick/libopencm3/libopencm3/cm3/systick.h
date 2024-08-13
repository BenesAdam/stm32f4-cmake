#ifndef SYSTICK_HPP
#define SYSTICK_HPP

#include "types.h"

#define rcc_ahb_frequency 0

bool systick_set_frequency(uint32_t freq, uint32_t ahb);
void systick_counter_enable(void);
void systick_interrupt_enable(void);

#endif /* SYSTICK_HPP */
