#include "acutest.h"
#include <cstdlib>
#include <cstdio>

#define private public
#include "bitset.hpp"
#define private private

class cTestBitset
{
public:
  static void Test_GetRequiredDataSize(void);
  static void Test_SetterAndGetter(void);
  static void Test_Flip(void);
  static void Test_BitCount(void);
  static void Test_NumberConstructor(void);
  static void Test_ToString(void);
};

TEST_LIST = {
    { "cTestBitset::Test_GetRequiredDataSize", cTestBitset::Test_GetRequiredDataSize },
    { "cTestBitset::Test_SetterAndGetter", cTestBitset::Test_SetterAndGetter },
    { "cTestBitset::Test_Flip", cTestBitset::Test_Flip },
    { "cTestBitset::Test_BitCount", cTestBitset::Test_BitCount },
    { "cTestBitset::Test_NumberConstructor", cTestBitset::Test_NumberConstructor },
    { "cTestBitset::Test_ToString", cTestBitset::Test_ToString },
    { NULL, NULL }
};

void cTestBitset::Test_GetRequiredDataSize(void)
{
  constexpr ui64 ui8Values = static_cast<ui64>(ui8_max) + 1U;
  constexpr ui64 ui16Values = static_cast<ui64>(ui16_max) + 1U;
  constexpr ui64 ui32Values = static_cast<ui64>(ui32_max) + 1U;

  TEST_CASE("ui8");
  // cBitset<0, ui8> is syntax error - no need to check this
  TEST_ASSERT((cBitset<1, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<3, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<8, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<9, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<15, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<16, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<17, ui8>::GetRequiredDataSize()) == 3U);
  TEST_ASSERT((cBitset<ui8Values, ui8>::GetRequiredDataSize()) == 32U);
  TEST_ASSERT((cBitset<ui16Values, ui8>::GetRequiredDataSize()) == 8192U);
  TEST_ASSERT((cBitset<ui32Values, ui8>::GetRequiredDataSize()) == 536870912U);

  TEST_CASE("ui16");
  TEST_ASSERT((cBitset<1, ui16>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<16, ui16>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<17, ui16>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<80, ui16>::GetRequiredDataSize()) == 5U);
  TEST_ASSERT((cBitset<81, ui16>::GetRequiredDataSize()) == 6U);
  TEST_ASSERT((cBitset<ui8Values, ui16>::GetRequiredDataSize()) == 16U);
  TEST_ASSERT((cBitset<ui16Values, ui16>::GetRequiredDataSize()) == 4096U);
  TEST_ASSERT((cBitset<ui32Values, ui16>::GetRequiredDataSize()) == 268435456U);

  TEST_CASE("ui32");
  TEST_ASSERT((cBitset<16, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<32, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<33, ui32>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<800, ui32>::GetRequiredDataSize()) == 25U);
  TEST_ASSERT((cBitset<801, ui32>::GetRequiredDataSize()) == 26U);
  TEST_ASSERT((cBitset<ui8Values, ui32>::GetRequiredDataSize()) == 8U);
  TEST_ASSERT((cBitset<ui16Values, ui32>::GetRequiredDataSize()) == 2048U);
  TEST_ASSERT((cBitset<ui32Values, ui32>::GetRequiredDataSize()) == 134217728U);

  TEST_CASE("ui64");
  TEST_ASSERT((cBitset<40, ui64>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<64, ui64>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<65, ui64>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<768, ui64>::GetRequiredDataSize()) == 12U);
  TEST_ASSERT((cBitset<769, ui64>::GetRequiredDataSize()) == 13U);
  TEST_ASSERT((cBitset<ui8Values, ui64>::GetRequiredDataSize()) == 4U);
  TEST_ASSERT((cBitset<ui16Values, ui64>::GetRequiredDataSize()) == 1024U);
  TEST_ASSERT((cBitset<ui32Values, ui64>::GetRequiredDataSize()) == 67108864U);
}

void cTestBitset::Test_SetterAndGetter(void)
{
  const ui64 indexAlwaysSet = 13;
  constexpr ui16 size = 256;
  cBitset<size, ui8> cut;
  TEST_CHECK(cut.Size() == size);

  auto TestBit = [&](const ui64 arg_index)
    {
      cut.Set(arg_index);
      TEST_CHECK(cut[arg_index] == true);
      TEST_CHECK(cut[indexAlwaysSet] == true);
      cut.Reset(arg_index);
      TEST_CHECK(cut[arg_index] == false);
      TEST_CHECK(cut[indexAlwaysSet] == true);
    };

  TEST_CASE("Index of always set bit.");
  cut.Set(indexAlwaysSet);
  TEST_CHECK(cut[indexAlwaysSet] == true);

  TEST_CASE("Test in range values.");
  TestBit(0);
  TestBit(6);
  TestBit(7);
  TestBit(8);
  TestBit(14);
  TestBit(15);
  TestBit(16);
  TestBit(150);
  TestBit(232);
  TestBit(255);

  TEST_CASE("Out of range value.");
  cut.Set(size);
  TEST_CHECK(cut[size] == false);

  TEST_CASE("Set/Reset all bits.");
  cut.Set();
  TEST_CHECK(cut.All() == true);
  cut.Reset();
  TEST_CHECK(cut.None() == true);
}

void cTestBitset::Test_Flip(void)
{
  const ui64 indexAlwaysSet = 15;
  constexpr ui16 size = 64;
  cBitset<size> cut;
  TEST_CHECK(cut.Size() == size);

  auto TestBit = [&](const ui64 arg_index)
    {
      cut.Flip(arg_index);
      TEST_CHECK(cut[arg_index] == true);
      TEST_CHECK(cut[indexAlwaysSet] == true);
      cut.Flip(arg_index);
      TEST_CHECK(cut[arg_index] == false);
      TEST_CHECK(cut[indexAlwaysSet] == true);
    };

  TEST_CASE("Index of always set bit.");
  cut.Set(indexAlwaysSet);
  TEST_CHECK(cut[indexAlwaysSet] == true);

  TEST_CASE("Test in range values.");
  TestBit(0);
  TestBit(30);
  TestBit(31);
  TestBit(32);
  TestBit(63);

  TEST_CASE("Out of range value.");
  cut.Flip(size);
  TEST_CHECK(cut[size] == false);
  cut.Flip(size);
  TEST_CHECK(cut[size] == false);
}

void cTestBitset::Test_BitCount(void)
{
  const ui8 expectedSetBits = 8U;
  const ui64 size = 25ULL;
  cBitset<size> cut;
  TEST_CHECK(cut.Size() == size);

  TEST_CASE("No bit is set.");
  TEST_CHECK(cut.Count() == 0ULL);
  TEST_CHECK(cut.Any() == false);
  TEST_CHECK(cut.None() == true);
  TEST_CHECK(cut.All() == false);

  TEST_CASE("Some bits are set.");
  for (ui64 i = 0U; i < expectedSetBits; i++)
  {
    cut.Set(i * 2ULL);
  }

  TEST_CHECK(cut.Count() == expectedSetBits);
  TEST_CHECK(cut.Any() == true);
  TEST_CHECK(cut.None() == false);
  TEST_CHECK(cut.All() == false);

  TEST_CASE("All bits are set.");
  for (ui64 i = 0U; i < size; i++)
  {
    cut.Set(i);
  }

  TEST_CHECK(cut.Count() == size);
  TEST_CHECK(cut.Any() == true);
  TEST_CHECK(cut.None() == false);
  TEST_CHECK(cut.All() == true);
}

void cTestBitset::Test_NumberConstructor(void)
{
  cBitset<8> cut8;

  cut8.Set(0);
  cut8.Set(2);
  TEST_CHECK(cut8.ToNumber() == 5);

  cut8.SetBits(5);
  TEST_CHECK(cut8.ToNumber() == 5);

  cut8.SetBits(0xAA);
  TEST_CHECK(cut8.ToNumber() == 0xAA);

  cut8.SetBits(0xFF);
  TEST_CHECK(cut8.ToNumber() == 0xFF);

  cBitset<16> cut16(0xAA55);
  TEST_CHECK(cut16.ToNumber() == 0xAA55);

  cut16.SetBits(0x55AA);
  TEST_CHECK(cut16.ToNumber() == 0x55AA);
}

void cTestBitset::Test_ToString(void)
{
  auto TestCase = [](const ui16 arg_value, const char* arg_expected)
    {
      TEST_CASE(arg_expected);
      cBitset<16> cut(arg_value);

      constexpr ui8 size = cut.GetStringSize();
      ui8 buffer[size];
      cut.ToString(buffer);

      TEST_CHECK(memcmp(&buffer[0], arg_expected, size) == 0);
    };

  TestCase(5,      "0000000000000101");
  TestCase(0x55AA, "0101010110101010");
  TestCase(0xAA55, "1010101001010101");
  TestCase(0x1234, "0001001000110100");
}
