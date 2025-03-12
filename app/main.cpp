extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
}

#include "types.h"
#include "systick.hpp"
#include "noinit_vars.hpp"

#define LED_PORT GPIOC
#define LED_PIN GPIO13

void InitLed(void);
void InitTimers(const ui32 arg_freqInHz);
void BlinkConfiguredFrequency(const ui32 arg_freqInHz);
void BlinkSawMode(void);
void UpdateFrequency(void);

volatile ui32 frequency = 500U;

int main(void)
{
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();

  InitLed();

  ui32 frequencies[] = {0U, 100U, 200U, 300U, 500U, 700U, 1000U, 1200};
  const ui8 size = sizeof(frequencies) / sizeof(frequencies[0U]);
  const ui8 index = nsNoinitVars::ResetCount % size;

  ui64 waitMicros = 0U;
  const ui64 cycleTimeUs = 30U * 1000U;

  if(index != 0U)
  {
    frequency = frequencies[index];
  }

  InitTimers(frequency);

  nsNoinitVars::ResetCount++;

  if(index != 0U)
  {
    BlinkConfiguredFrequency(frequency);
  }
  else
  {
    BlinkSawMode();
  }

  while (true)
  {
    cSysTick::DelayUs(waitMicros);

    const ui64 start = cSysTick::Micros();

    if(index == 0U)
    {
      UpdateFrequency();
    }

    const ui64 end = cSysTick::Micros();
    waitMicros = cycleTimeUs - (end - start);
  }

  return 0;
}

void InitLed(void)
{
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
  gpio_set(LED_PORT, LED_PIN);
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

void InitTimers(const ui32 arg_freqInHz)
{
  const ui32 period = (1000000U / arg_freqInHz);

  // TIM2 -> interrupt every 2ms (500Hz)
  rcc_periph_clock_enable(RCC_TIM2);
  timer_direction_down(TIM2);
  timer_set_prescaler(TIM2, 95);
  timer_set_period(TIM2, period - 1U);
  timer_enable_preload(TIM2);
  timer_enable_irq(TIM2, TIM_DIER_UIE);
  nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_set_priority(NVIC_TIM2_IRQ, 2);
  timer_enable_counter(TIM2);

  // PA0 controlled by TIM5 CH1
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);
  gpio_set_af(GPIOA, GPIO_AF2, GPIO0);

  // TIM5 -> generate 45us pulse
  rcc_periph_clock_enable(RCC_TIM5);

  timer_disable_counter(TIM5);
  timer_set_prescaler(TIM5, 95);
  timer_set_period(TIM5, 45);

  timer_set_oc_mode(TIM5, TIM_OC1, TIM_OCM_PWM2);
  timer_enable_oc_output(TIM5, TIM_OC1);
  timer_set_oc_value(TIM5, TIM_OC1, 1);

  timer_one_shot_mode(TIM5);
}

void BlinkConfiguredFrequency(const ui32 arg_freqInHz)
{
  cSysTick::DelayMs(150U);

  const ui8 blinkCount = arg_freqInHz / 100U;

  for(ui8 i = 0U; i < blinkCount; i++)
  {
    cSysTick::DelayMs(150U);
    gpio_clear(LED_PORT, LED_PIN);
    cSysTick::DelayMs(250U);
    gpio_set(LED_PORT, LED_PIN);
  }

  cSysTick::DelayMs(700U);
}

void BlinkSawMode(void)
{
  cSysTick::DelayMs(150U);

  for(ui8 i = 0U; i < 5U; i++)
  {
    cSysTick::DelayMs(50U);
    gpio_clear(LED_PORT, LED_PIN);
    cSysTick::DelayMs(50U);
    gpio_set(LED_PORT, LED_PIN);
  }
}

void UpdateFrequency(void)
{
  const ui32 lowerLimit = 450U;
  const ui32 upperLimit = 550U;
  static si8 step = -1;

  // Update frequency
  frequency += step;

  // Update direction
  if ((frequency <= lowerLimit) || (frequency >= upperLimit))
  {
    step *= -1;

    // Turn on LED when speeding up
    if(step == 1)
    {
      gpio_clear(LED_PORT, LED_PIN);
    }
    else
    {
      gpio_set(LED_PORT, LED_PIN);
    }
  }

  // Update timer period
  const ui32 period = (1000000U / frequency);
  timer_set_period(TIM2, period - 1U);
}
