#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "types.h"
#include "display_character_map.hpp"

// Defines
#define LCD_BIT(x) (1U << (x))

/* Command bits */
#define LCD_BIT_RS LCD_BIT(0U) // Register select Instruction(0)/Data(1)
#define LCD_BIT_RW LCD_BIT(1U) // Write(0)/Read(1)
#define LCD_BIT_E LCD_BIT(2U) // Enable pin
#define LCD_BIT_BL LCD_BIT(3U) // Backlight lin

#define LCD_BIT_NO_BL 0U

// 8-bit instructions
/* Instructions list*/
#define LCD_INST_CLEAR_DISPLAY LCD_BIT(0U)
#define LCD_INST_RETURN_HOME LCD_BIT(1U)
#define LCD_INST_ENTRY_MODE_SET LCD_BIT(2U)
#define LCD_INST_DISPLAY_ONOFF LCD_BIT(3U)
#define LCD_INST_CURSOR_DISPLAY_SHIFT LCD_BIT(4U)
#define LCD_INST_FUNCTION_SET LCD_BIT(5U)
#define LCD_INST_SET_CG_RAM LCD_BIT(6U)
#define LCD_INST_SET_DD_RAM LCD_BIT(7U)
#define LCD_INST_READ_BUSY_OR_ADDR LCD_BIT(8U)

/* Entry mode options */
#define LCD_ENTRY_MODE_ID_INCREMENT LCD_BIT(1U)
#define LCD_ENTRY_MODE_SHIFT_ENABLED LCD_BIT(0U)

/* Display on/off constrol options */
#define LCD_ONOFF_ALL_ON LCD_BIT(3U)
#define LCD_ONOFF_DISPLAY_ON LCD_BIT(2U)
#define LCD_ONOFF_CURSOR_ON LCD_BIT(1U)
#define LCD_ONOFF_BLINK_CURSOR_ON LCD_BIT(0U)

/* Cursor or display shift */
#define LCD_SHIFT_DISPLAY LCD_BIT(3U)
#define LCD_SHIFT_TO_RIGHT LCD_BIT(2U)

/* Function set */
#define LCD_FSET_DATA_LEN_8BIT LCD_BIT(4U)
#define LCD_FSET_DATA_LEN_4BIT 0U
#define LCD_FSET_NUM_LINES_2 LCD_BIT(3U)
#define LCD_FSET_NUM_LINES_1 0U
#define LCD_FSET_CHAR_FONT_5x10 LCD_BIT(2U)
#define LCD_FSET_CHAR_FONT_5x7 0U

/* Read busy flag & address */
#define LCD_READ_BF_ADDR_BF LCD_BIT(7U)

class cDisplay
{
public:
  cDisplay(void);
  void Init(const ui8 arg_i2c_address);
  void Clear(void);

  // Print character(s)
  void Print(const eDisplayCharacter arg_character);
  void Print(const wchar_t arg_character);
  void Print(const wchar_t* arg_string, const uint64_t arg_pauseInMs = 0U);

  // Cursor
  void SetCursor(const ui8 arg_row, const ui8 arg_column);
  void Home(void);
  void NewLine(void);

  // Backlight
  void TurnBacklightOn(void);
  void TurnBacklightOff(void);

  // Dimensions
  ui8 Height(void) const;
  ui8 Width(void) const;
  ui8 CharacterHeight(void) const;
  ui8 CharacterWidth(void) const;

  // Custom Symbol
  void SetCustomSymbol(const eDisplayCharacter arg_character, const ui8 arg_matrix[]);

private:
  void WriteByte(const ui8 arg_data, const ui8 arg_mode);
  void WriteNibble(const ui8 arg_byte);
  void WriteExpander(const ui8 arg_byte);

  ui8 address;
  ui8 backlight;
};

#endif /* DISPLAY_HPP */
