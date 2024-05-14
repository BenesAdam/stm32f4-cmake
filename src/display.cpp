#include "display.hpp"
#include "i2c1.hpp"
#include "systick.hpp"

cDisplay::cDisplay(void) : address(0U), backlight(LCD_BIT_NO_BL)
{
}

void cDisplay::Init(const uint8_t arg_i2c_address)
{
  address = arg_i2c_address;

  // DelayMs 15ms
  cSysTick::DelayMs(20U);

  // First init command - start with 8-bit try to set it to 4-bit mode
  WriteNibble(0x3U << 4U);
  cSysTick::DelayUs(4300U);

  // Second init command
  WriteNibble(0x3U << 4U);
  cSysTick::DelayUs(300U);

  // Third init command
  WriteNibble(0x3U << 4U);
  cSysTick::DelayUs(130U);

  // Finally set it to 4-bit
  WriteNibble(0x2U << 4U);

  // Function set - 4bit, 2 lines, font 5x7 dots
  WriteByte(LCD_INST_FUNCTION_SET | LCD_FSET_DATA_LEN_4BIT | LCD_FSET_NUM_LINES_2 | LCD_FSET_CHAR_FONT_5x7, 0U);

  // Display off
  WriteByte(LCD_INST_DISPLAY_ONOFF | LCD_ONOFF_DISPLAY_ON, 0U);

  // Display clear
  WriteByte(LCD_INST_CLEAR_DISPLAY, 0U);
  cSysTick::DelayUs(1600U);

  // Entry mode - increment, no shift
  backlight = LCD_BIT_BL;
  WriteByte(LCD_INST_ENTRY_MODE_SET | LCD_ENTRY_MODE_ID_INCREMENT, 0U);
}

void cDisplay::Clear(void)
{
  WriteByte(LCD_INST_CLEAR_DISPLAY, 0U);
  cSysTick::DelayUs(1650U);
}

void cDisplay::Print(const eDisplayCharacter arg_character)
{
  WriteByte(static_cast<uint8_t>(arg_character), LCD_BIT_RS);
}

void cDisplay::Print(const wchar_t arg_character)
{
  if (arg_character != L'\n')
  {
    Print(GetDisplayCharacter(arg_character));
  }
  else
  {
    NewLine();
  }
}

void cDisplay::Print(const wchar_t *arg_string, const uint64_t arg_pauseInMs)
{
  uint32_t i = 0U;
  while (arg_string[i] != L'\0')
  {
    if (arg_pauseInMs != 0U)
    {
      cSysTick::DelayMs(arg_pauseInMs);
    }

    wchar_t character = arg_string[i];
    Print(character);

    i++;
  }
}

void cDisplay::SetCursor(const uint8_t arg_row, const uint8_t arg_column)
{
  if ((arg_row >= Height()) || (arg_column >= Width()))
  {
    return;
  }

  uint8_t code = ((arg_row == 0U) ? 0x0U : 0x40U) + arg_column;
  WriteByte(LCD_INST_SET_DD_RAM | code, 0U);
}

void cDisplay::Home(void)
{
  SetCursor(0U, 0U);
}

void cDisplay::NewLine(void)
{
  SetCursor(1U, 0U);
}

void cDisplay::TurnBacklightOn(void)
{
  backlight = LCD_BIT_BL;
  WriteExpander(0U);
}

void cDisplay::TurnBacklightOff(void)
{
  backlight = LCD_BIT_NO_BL;
  WriteExpander(0U);
}

uint8_t cDisplay::Height(void) const
{
  return 2U;
}

uint8_t cDisplay::Width(void) const
{
  return 16U;
}

uint8_t cDisplay::CharacterHeight(void) const
{
  return 8U;
}

uint8_t cDisplay::CharacterWidth(void) const
{
  return 5U;
}

void cDisplay::SetCustomSymbol(const eDisplayCharacter arg_character, const uint8_t arg_matrix[])
{
  // Check display character
  if (arg_character > eDisplayCharacter::CUSTOM_8)
  {
    return;
  }

  // Set Character Generator RAM address
  const uint8_t address = (static_cast<uint8_t>(arg_character) & 0b111U) << 3U;
  WriteByte(LCD_INST_SET_CG_RAM | address, 0U);

  // Fill CG RAM row by row
  for (uint8_t i = 0U; i < CharacterHeight(); i++)
  {
    const uint8_t mask = (1U << CharacterWidth()) - 1U;
    WriteByte(arg_matrix[i] & mask, LCD_BIT_RS);
  }
}

void cDisplay::WriteByte(const uint8_t arg_data, const uint8_t arg_mode)
{
  WriteNibble((arg_data & 0xF0) | arg_mode);
  WriteNibble((arg_data << 4U) | arg_mode);
  cSysTick::DelayUs(40U);
}

void cDisplay::WriteNibble(const uint8_t arg_byte)
{
  WriteExpander(arg_byte | LCD_BIT_E);
  cSysTick::DelayUs(1U);
  WriteExpander(arg_byte & (~LCD_BIT_E));
  cSysTick::DelayUs(50U);
}

void cDisplay::WriteExpander(const uint8_t arg_byte)
{
  cI2C1::WriteByte(address, arg_byte | backlight);
}
