#include "systick.hpp"
#include "i2c1.hpp"
#include "display.hpp"
#include "noinit_vars.hpp"
#include "errorhandler_inst.hpp"

extern "C"
{
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
}

#define LED_PORT GPIOC
#define RCC_LED_PORT RCC_GPIOC
#define LED_PIN GPIO13

#define I2C_PORT GPIOB
#define RCC_I2C_PORT RCC_GPIOB
#define I2C_SCL1_PIN GPIO8
#define I2C_SDA1_PIN GPIO9

#define BATTERY eDisplayCharacter::CUSTOM_1

cDisplay display;

void AddBatterySymbol(void)
{
  const ui8 batteryBitMap[] = 
  {
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
  };

  display.SetCustomSymbol(eDisplayCharacter::CUSTOM_1, batteryBitMap);
}

void PrintDisplayDemo(void)
{
  display.TurnBacklightOff();
  display.Clear();

  for (ui8 i = 0U; i < display.Height(); i++)
  {
    for (ui8 j = 0U; j < display.Width(); j++)
    {
      const eDisplayCharacter character = (((j + i) % 2) == 0) ? BATTERY : eDisplayCharacter::BLANK;
      display.Print(character);
    }

    display.Print(L'\n');
  }
  display.TurnBacklightOn();

  display.SetCursor(0U, 3U);
  display.Print(L" Resets: ", 20);
  display.SetCursor(1U, 5U);

  display.Print(nsNoinitVars::ResetCount, 20);
  cSysTick::DelayMs(1000);

  display.TurnBacklightOff();
  cSysTick::DelayMs(250);
  display.TurnBacklightOn();

  cSysTick::DelayMs(2000);
}

int main()
{
  nsNoinitVars::ResetCount++;
  nsNoinitVars::InitializeOnColdStart();
  cSysTick::Setup();
  cI2C1::Setup();

  //display.Init(0x27);
  //AddBatterySymbol();
  //display.Clear();

  rcc_periph_clock_enable(RCC_LED_PORT);
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

  while (true)
  {
    //PrintDisplayDemo();
    ErrorHandler.Cyclic();    
  }
}
