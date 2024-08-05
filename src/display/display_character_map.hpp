#ifndef DISPLAY_CHARACTER_MAP_HPP
#define DISPLAY_CHARACTER_MAP_HPP

enum class eDisplayCharacter
{
  // Column 0000
  CUSTOM_1 = 0b00000000,
  CUSTOM_2 = 0b00000001,
  CUSTOM_3 = 0b00000010,
  CUSTOM_4 = 0b00000011,
  CUSTOM_5 = 0b00000100,
  CUSTOM_6 = 0b00000101,
  CUSTOM_7 = 0b00000110,
  CUSTOM_8 = 0b00000111,

  // Column 0010
  SPACE = 0b00100000,
  EXCLAMATION_MARK = 0b00100001,
  DOUBLE_QUOTE = 0b00100010,
  HASH = 0b00100011,
  DOLLAR = 0b00100100,
  PERCENT = 0b00100101,
  AMPERSAND = 0b00100110,
  APOSTROPHE = 0b00100111,
  LEFT_PARENTHESIS = 0b00101000,
  RIGHT_PARENTHESIS = 0b00101001,
  ASTERISK = 0b00101010,
  PLUS = 0b00101011,
  COMMA = 0b00101100,
  HYPHEN = 0b00101101,
  PERIOD = 0b00101110,
  SLASH = 0b00101111,

  // Column 0011
  ZERO = 0b00110000,
  ONE = 0b00110001,
  TWO = 0b00110010,
  THREE = 0b00110011,
  FOUR = 0b00110100,
  FIVE = 0b00110101,
  SIX = 0b00110110,
  SEVEN = 0b00110111,
  EIGHT = 0b00111000,
  NINE = 0b00111001,
  COLON = 0b00111010,
  SEMICOLON = 0b00111011,
  LESS_THAN = 0b00111100,
  EQUALS = 0b00111101,
  GREATER_THAN = 0b00111110,
  QUESTION_MARK = 0b00111111,

  // Column 0100
  AT_SIGN = 0b01000000,
  A = 0b01000001,
  B = 0b01000010,
  C = 0b01000011,
  D = 0b01000100,
  E = 0b01000101,
  F = 0b01000110,
  G = 0b01000111,
  H = 0b01001000,
  I = 0b01001001,
  J = 0b01001010,
  K = 0b01001011,
  L = 0b01001100,
  M = 0b01001101,
  N = 0b01001110,
  O = 0b01001111,

  // Column 0101
  P = 0b01010000,
  Q = 0b01010001,
  R = 0b01010010,
  S = 0b01010011,
  T = 0b01010100,
  U = 0b01010101,
  V = 0b01010110,
  W = 0b01010111,
  X = 0b01011000,
  Y = 0b01011001,
  Z = 0b01011010,
  LEFT_BRACKET = 0b01011011,
  YEN = 0b01011100,
  RIGHT_BRACKET = 0b01011101,
  CARET = 0b01011110,
  UNDERSCORE = 0b01011111,

  // Column 0110
  BACKTICK = 0b01100000,
  a = 0b01100001,
  b = 0b01100010,
  c = 0b01100011,
  d = 0b01100100,
  e = 0b01100101,
  f = 0b01100110,
  g = 0b01100111,
  h = 0b01101000,
  i = 0b01101001,
  j = 0b01101010,
  k = 0b01101011,
  l = 0b01101100,
  m = 0b01101101,
  n = 0b01101110,
  o = 0b01101111,

  // Column 0111
  p = 0b01110000,
  q = 0b01110001,
  r = 0b01110010,
  s = 0b01110011,
  t = 0b01110100,
  u = 0b01110101,
  v = 0b01110110,
  w = 0b01110111,
  x = 0b01111000,
  y = 0b01111001,
  z = 0b01111010,
  LEFT_BRACE = 0b01111011,
  PIPE = 0b01111100,
  RIGHT_BRACE = 0b01111101,
  RIGHT_ARROW = 0b01111110,
  LEFT_ARROW = 0b01111111,

  // Column 1010
  BLANK = 0b10100000,
  DOT = 0b10100001,

  // Column 1101
  DEGREE = 0b11011111,

  // Column 1110
  alpha = 0b11100000,
  a_umlaut = 0b11100001,
  beta = 0b11100010,
  epsilon = 0b11100011,
  micro = 0b11100100,
  sigma = 0b11100101,
  rho = 0b11100110,
  theta = 0b11100111,
  
  // Column 1111
  THETA = 0b11110010,
  OMEGA = 0b11110100,
  SIGMA = 0b11110110,
  pi = 0b11110111,
  DIVISION = 0b11111101,
  FULL = 0b11111111
};

bool DisplayCharacterExists(const wchar_t arg_character);
eDisplayCharacter GetDisplayCharacter(const wchar_t arg_character);

#endif /* DISPLAY_CHARACTER_MAP_HPP */