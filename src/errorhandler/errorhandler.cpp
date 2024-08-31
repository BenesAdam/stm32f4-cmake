#include "errorhandler.hpp"

const sErrorConfig cErrorHandler::ErrorConfig[E_NUMBER_OF_ERRORS] =
{
// Error number         |       Error enable        |       Reaction
{  E_FREE_TO_USE,              E_ERROR_ENABLED,            NONE_REACT}
};

sErrorFlags cErrorHandler::ErrorFlags[E_NUMBER_OF_ERRORS];

cErrorHandler::cErrorHandler(void) :
  clearErrorTable(false),
  lastFreePosition(0U)
{
  ClearErrorTable();
}

void cErrorHandler::SetErrorActive(eErrors arg_errorNumber)
{
  SetError(arg_errorNumber, true);
}

void cErrorHandler::SetErrorInactive(eErrors arg_errorNumber)
{
  SetError(arg_errorNumber, false);
}

void cErrorHandler::SetError(eErrors arg_errorNumber, bool arg_isErrorActive)
{
  ErrorFlags[static_cast<ui8>(arg_errorNumber)].active = arg_isErrorActive;
}

void cErrorHandler::Cyclic(void)
{
  for(ui8 errorIndex = 0U; errorIndex < E_NUMBER_OF_ERRORS; errorIndex++)
  {
    if(WouldErrorBeProccessed(errorIndex))
      {
        AddToErrorTable(errorIndex);
        ErrorConfig[errorIndex].reaction->ErrorReaction();
      }
  }

  if (clearErrorTable)
  {
    ClearErrorTable();
  }
}

bool cErrorHandler::WouldErrorBeProccessed(const ui8 arg_errorIndex)
{
  return ((ErrorConfig[arg_errorIndex].enable == E_ERROR_ENABLED) && 
          (ErrorFlags[arg_errorIndex].active == true) && 
          (ErrorFlags[arg_errorIndex].isAlreadyWritten == false) &&
          (lastFreePosition < errorTableSize));
}

void cErrorHandler::AddToErrorTable(const ui8 arg_errorIndex)
{
  errorTable[lastFreePosition] = ErrorConfig[arg_errorIndex].errorNumber;
  ErrorFlags[arg_errorIndex].isAlreadyWritten = true;
  lastFreePosition++;
}

void cErrorHandler::ClearErrorTable(void)
{
  for(ui8 errorTableIndex = 0U; errorTableIndex < errorTableSize; errorTableIndex++)
  {
    errorTable[errorTableIndex] = E_NO_ERROR;
  }

  for(ui8 errorIndex = 0U; errorIndex < E_NUMBER_OF_ERRORS; errorIndex++)
  {
    ErrorFlags[errorIndex].isAlreadyWritten = false;
  }
  clearErrorTable = false;
  lastFreePosition = 0U;
}
