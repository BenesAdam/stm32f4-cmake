#ifndef RESET_HPP
#define RESET_HPP

#include "types.h"

enum eResetReason
{
  E_NO_RESET = 0U,
  E_ERRORHANDLER_REACTION = 1U
};

class cReset
{
public:
  cReset(void);

  void SoftwareReset(const eResetReason arg_resetNumber, const ui16 arg_resetType);

private:

  NOINIT static volatile eResetReason ResetNumber;
  NOINIT static volatile ui16 ResetType;
};

#endif /* RESET_HPP */
