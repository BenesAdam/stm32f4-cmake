#ifndef RCC_HPP
#define RCC_HPP

#include "types.h"

#define RCC_GPIOB 0
#define RCC_I2C1 0

typedef struct rcc_clock_scale
{
  ui16 dummy;
} rcc_clock_scale;

#define RCC_CLOCK_3V3_96MHZ 0

extern rcc_clock_scale rcc_hsi_configs[1];

void rcc_periph_clock_enable(ui32 clken);
void gpio_mode_setup(ui32 gpioport, ui8 mode, ui8 pull_up_down,
                     ui16 gpios);
void gpio_set_output_options(ui32 gpioport, ui8 otype, ui8 speed,
                             ui16 gpios);
void gpio_set_af(ui32 gpioport, ui8 alt_func_num, ui16 gpios);

void rcc_clock_setup_pll(const struct rcc_clock_scale *clock);

#endif /* RCC_HPP */
