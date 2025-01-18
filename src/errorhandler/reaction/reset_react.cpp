#include "reset_react.hpp"
#include "reset_inst.hpp"

cResetReact::cResetReact(void)
{
}

void cResetReact::ErrorReaction(const ui16 arg_errorNumber)
{
  Reset.SoftwareReset(E_ERRORHANDLER_REACTION, arg_errorNumber);
}
