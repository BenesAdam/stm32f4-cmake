extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}

#include "systick.hpp"
#include "i2c1.hpp"
#include "display.hpp"
#include "noinit_vars.hpp"

const ui64 MorseShortDelayTime = 150U; // milliseconds
const ui64 MorseLongDelayTime = 3U * MorseShortDelayTime;
const ui64 MorseEndOfLetterTime = (3U - 1U) * MorseShortDelayTime;
const ui64 MorseEndOfWordTime = (7U - 1U) * MorseShortDelayTime;

#define BOLD_QUESTIONMARK_SYMBOL eDisplayCharacter::CUSTOM_1
#define LED_PORT GPIOC
#define LED_PIN GPIO13

void BlinkShort(void);
void BlinkLong(void);
void InitializeLight(void);
void BlinkMorseCode(void);

void InitializeDisplay(void);
void HandleResets(void);
void AskQuestion(void);
void ShowTimer(void);

cDisplay display;

int main(void)
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();
  cI2C1::Setup();
  InitializeDisplay();
  InitializeLight();

  BlinkShort();
  HandleResets();
  AskQuestion();

  while (1)
  {
    ShowTimer();
  }

  return 0;
}

void BlinkShort(void)
{
  gpio_clear(LED_PORT, LED_PIN);
  cSysTick::DelayMs(MorseShortDelayTime);
  gpio_set(LED_PORT, LED_PIN);
  cSysTick::DelayMs(MorseShortDelayTime);
}

void BlinkLong(void)
{
  gpio_clear(LED_PORT, LED_PIN);
  cSysTick::DelayMs(MorseLongDelayTime);
  gpio_set(LED_PORT, LED_PIN);
  cSysTick::DelayMs(MorseShortDelayTime);
}

void InitializeLight(void)
{
  rcc_periph_clock_enable(RCC_GPIOC);

#ifdef STM32F4
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
#else
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
#endif
}

void BlinkMorseCode(void)
{
  // S
  BlinkShort();
  BlinkShort();
  BlinkShort();
  cSysTick::DelayMs(MorseEndOfLetterTime);

  // O
  BlinkLong();
  BlinkLong();
  BlinkLong();
  cSysTick::DelayMs(MorseEndOfLetterTime);

  // S
  BlinkShort();
  BlinkShort();
  BlinkShort();
  cSysTick::DelayMs(MorseEndOfLetterTime);

  // End of the word
  cSysTick::DelayMs(MorseEndOfWordTime);
}

void InitializeDisplay(void)
{
  display.Init(0x27U);
  const ui8 questionMarkBitMap[] =
      {
          0b11111,
          0b11011,
          0b10001,
          0b00111,
          0b00100,
          0b00000,
          0b01110,
          0b01110,
      };
  display.SetCustomSymbol(BOLD_QUESTIONMARK_SYMBOL, questionMarkBitMap);
  display.Clear();
}

void HandleResets(void)
{
  if (nsNoinitVars::ResetCount == 0U)
  {
    return;
  }

  display.Print(L"Another reset...\n");
  display.Print(L"Count: ");
  display.Print(static_cast<ui32>(nsNoinitVars::ResetCount));
  const ui32 waitAfterResetCount = (nsNoinitVars::ResetCount > 1U) ? 1000U : 2000U;
  cSysTick::DelayMs(waitAfterResetCount);
  display.Clear();

  if ((nsNoinitVars::ResetCount) % 3U == 0U)
  {
    display.Print(L"Are you mad");

    for (ui8 i = 0U; i < 3U; i++)
    {
      display.Print(BOLD_QUESTIONMARK_SYMBOL);
    }

    display.NewLine();
    display.Print(L"You are crazy...");

    for (ui8 i = 0U; i < 3U; i++)
    {
      BlinkMorseCode();
    }
  }

  display.Print(L"Anyway...");
  cSysTick::DelayMs(1000);

  display.Clear();
}

void AskQuestion(void)
{
  display.Print(L"How long can you\n");
  display.Print(L"...not blink?");
  const ui32 waitAfterQuestion = (nsNoinitVars::ResetCount == 0U) ? 3000U : 1000;
  cSysTick::DelayMs(waitAfterQuestion);
}

void ShowTimer(void)
{
  static ui64 startMeasuringTime = cSysTick::Millis();
  static ui64 nextToggleTime = 0U;

  display.Clear();
  display.Print(L"Blinkless for:\n");

  ui64 currentTime = cSysTick::Millis() - startMeasuringTime;
  ui32 seconds = currentTime / 1000ULL;
  ui32 millis = currentTime % 1000ULL;

  display.Print(seconds);
  display.Print(eDisplayCharacter::PERIOD);
  display.Print(millis);

  if (currentTime >= nextToggleTime)
  {
    gpio_toggle(LED_PORT, LED_PIN);
    nextToggleTime = currentTime + 1000ULL;
  }

  cSysTick::DelayMs(150);
}
