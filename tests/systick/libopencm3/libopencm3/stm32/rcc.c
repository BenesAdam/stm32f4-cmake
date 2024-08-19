#include "rcc.h"

rcc_clock_scale rcc_hsi_configs[1] = 
{
  {0U}
};

void rcc_periph_clock_enable(ui32 clken)
{
}

void gpio_mode_setup(ui32 gpioport, ui8 mode, ui8 pull_up_down, ui16 gpios)
{
}

void gpio_set_output_options(ui32 gpioport, ui8 otype, ui8 speed, ui16 gpios)
{
}

void gpio_set_af(ui32 gpioport, ui8 alt_func_num, ui16 gpios)
{
}

void rcc_clock_setup_pll(const struct rcc_clock_scale *clock)
{

}
