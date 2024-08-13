#include "acutest.h"

#include "systick.hpp"

void Test_Setup(void)
{
  cSysTick::Setup();
}

void Test_Millis(void)
{
  cSysTick::Millis();
}

TEST_LIST = {
    { "Test_Setup", Test_Setup },
    { "Test_Millis", Test_Millis },
    { NULL, NULL }
};
