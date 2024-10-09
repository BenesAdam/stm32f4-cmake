#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include "types.h"
#include "errorhandler_types.hpp"

class cErrorHandler
{
public:

  cErrorHandler(void);

  void Cyclic(void);
  void SetErrorActive(eErrors arg_errorNumber);
  void SetErrorInactive(eErrors arg_errorNumber);
  void SetError(eErrors arg_errorNumber, bool arg_isErrorActive);

private:

  bool WouldErrorBeProccessed(const ui8 arg_errorIndex);
  void AddToErrorTable(const ui8 arg_errorIndex);
  void ClearErrorTable(void);

  volatile eErrors errorTable[errorTableSize];
  bool clearErrorTable;
  ui8 lastFreePosition;
  static const sErrorConfig ErrorConfig[E_NUMBER_OF_ERRORS];
  static sErrorFlags ErrorFlags[E_NUMBER_OF_ERRORS];
};

#endif  /* #ifndef ERRORHANDLER_HPP */
