#include "rcc.h"

rcc_clock_scale rcc_hsi_configs[1] = 
{
  {0U}
};

void rcc_periph_clock_enable(uint32_t clken)
{
}

void gpio_mode_setup(uint32_t gpioport, uint8_t mode, uint8_t pull_up_down, uint16_t gpios)
{
}

void gpio_set_output_options(uint32_t gpioport, uint8_t otype, uint8_t speed, uint16_t gpios)
{
}

void gpio_set_af(uint32_t gpioport, uint8_t alt_func_num, uint16_t gpios)
{
}

void rcc_clock_setup_pll(const struct rcc_clock_scale *clock)
{

}
