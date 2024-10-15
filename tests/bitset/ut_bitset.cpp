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
  TEST_CASE("ui8");
  TEST_ASSERT((cBitset<1, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<3, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<8, ui8>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<9, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<15, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<16, ui8>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<17, ui8>::GetRequiredDataSize()) == 3U);

  TEST_CASE("ui16");
  TEST_ASSERT((cBitset<1, ui16>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<16, ui16>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<17, ui16>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<80, ui16>::GetRequiredDataSize()) == 5U);
  TEST_ASSERT((cBitset<81, ui16>::GetRequiredDataSize()) == 6U);

  TEST_CASE("ui32");
  TEST_ASSERT((cBitset<16, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<32, ui32>::GetRequiredDataSize()) == 1U);
  TEST_ASSERT((cBitset<33, ui32>::GetRequiredDataSize()) == 2U);
  TEST_ASSERT((cBitset<800, ui32>::GetRequiredDataSize()) == 25U);
  TEST_ASSERT((cBitset<801, ui32>::GetRequiredDataSize()) == 26U);
}
