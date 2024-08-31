#include "errorhandler_types.hpp"

cNoneReact NoneReact;
cResetReact ResetReact;

sErrorFlags::sErrorFlags()
{
  active = false;
  isAlreadyWritten = false;
}
