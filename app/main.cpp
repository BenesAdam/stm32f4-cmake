#include "types.h"
#include "header_checker.hpp"

volatile bool startApp = true;

int main(void)
{
  while (!startApp) {}
  if (HeaderChecker.FindApplication())
  {
    if (HeaderChecker.CheckApplicationHeader())
    {
      cHeaderChecker::RunApplication();
    }
  }
  else
  {
    while (true) {}
  }

  return 0;
}
