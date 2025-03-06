extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
}

#include "systick.hpp"

void InitTimers(void);

int main(void)
{
  cSysTick::Setup();
  InitTimers();

  while (true)
  {
  }

  return 0;
}

extern "C" void tim2_isr(void)
{
  if (timer_get_flag(TIM2, TIM_SR_UIF))
  {
    timer_clear_flag(TIM2, TIM_SR_UIF);

    timer_disable_counter(TIM5);
    timer_generate_event(TIM5, TIM_EGR_UG);
    timer_enable_counter(TIM5);
  }
}

void InitTimers(void)
{
  // TIM2 -> interrupt every 2ms (500Hz)
  rcc_periph_clock_enable(RCC_TIM2);
  timer_set_prescaler(TIM2, 95);
  timer_set_period(TIM2, 1999);
  timer_enable_irq(TIM2, TIM_DIER_UIE);
  nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_set_priority(NVIC_TIM2_IRQ, 2);
  timer_enable_counter(TIM2);

  // PA0 controlled by TIM5 CH1
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);
  gpio_set_af(GPIOA, GPIO_AF2, GPIO0);

  // TIM -> generate 45us pulse
  rcc_periph_clock_enable(RCC_TIM5);

  timer_disable_counter(TIM5);
  timer_set_prescaler(TIM5, 95);
  timer_set_period(TIM5, 45);

  timer_set_oc_mode(TIM5, TIM_OC1, TIM_OCM_PWM2);
  timer_enable_oc_output(TIM5, TIM_OC1);
  timer_set_oc_value(TIM5, TIM_OC1, 1);

  timer_one_shot_mode(TIM5);
}
