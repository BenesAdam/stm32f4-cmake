#include "types.h"
#include "systick.hpp"
#include "header_checker.hpp"

volatile ui32 startApp = 0x55AA55AAU;

int main(void)
{
  while (startApp != 0x55AA55AAU) {}
  
  cSysTick::Setup();
  HeaderChecker.CheckApplication();

  if (HeaderChecker.IsAppValidated())
  {
    HeaderChecker.RunApplication();
  }

  return 0;
}
