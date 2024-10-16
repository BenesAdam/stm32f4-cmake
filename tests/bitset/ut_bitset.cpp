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
};

TEST_LIST = {
    { "cTestBitset::Test_GetRequiredDataSize", cTestBitset::Test_GetRequiredDataSize },
    { NULL, NULL }
};

void cTestBitset::Test_GetRequiredDataSize(void)
{
  constexpr ui64 ui8Values = static_cast<ui64>(ui8_max) + 1U;
  constexpr ui64 ui16Values = static_cast<ui64>(ui16_max) + 1U;
  constexpr ui64 ui32Values = static_cast<ui64>(ui32_max) + 1U;

  TEST_CASE("ui8");
  TEST_ASSERT((cBitset<0, ui8>::GetRequiredDataSize()) == 0U);
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
  TEST_ASSERT((cBitset<0, ui32>::GetRequiredDataSize()) == 0U);
  TEST_ASSERT((cBitset<16, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<32, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<33, ui32>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<800, ui32>::GetRequiredDataSize()) == 25U);
  TEST_ASSERT((cBitset<801, ui32>::GetRequiredDataSize()) == 26U);
  TEST_ASSERT((cBitset<ui8Values, ui32>::GetRequiredDataSize()) == 8U);
  TEST_ASSERT((cBitset<ui16Values, ui32>::GetRequiredDataSize()) == 2048U);
  TEST_ASSERT((cBitset<ui32Values, ui32>::GetRequiredDataSize()) == 134217728U);

  TEST_CASE("ui64");
  TEST_ASSERT((cBitset<0, ui64>::GetRequiredDataSize()) == 0U);
  TEST_ASSERT((cBitset<40, ui64>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<64, ui64>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<65, ui64>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<768, ui64>::GetRequiredDataSize()) == 12U);
  TEST_ASSERT((cBitset<769, ui64>::GetRequiredDataSize()) == 13U);
  TEST_ASSERT((cBitset<ui8Values, ui64>::GetRequiredDataSize()) == 4U);
  TEST_ASSERT((cBitset<ui16Values, ui64>::GetRequiredDataSize()) == 1024U);
  TEST_ASSERT((cBitset<ui32Values, ui64>::GetRequiredDataSize()) == 67108864U);
}
