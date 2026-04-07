#ifndef HEADER_CHECKER_HPP
#define HEADER_CHECKER_HPP

#include "types.h"
#include "header_structure.hpp"

class cHeaderChecker
{
public:
  cHeaderChecker(void);
  void CheckApplication(void);
  bool IsAppValidated(void);
  static void RunApplication(void) __attribute__((noreturn));

private:
  bool FindApplication(void);
  bool CheckSignature(void);
  static ui32 HeaderAddress;
  sHeader* Header;
  ui8 Hash[32U];
  ui8 Hash_from_sig[32U];
  bool AppFounded;
  bool SignitureRight;
  ui32 AppRuntime;
};

extern cHeaderChecker HeaderChecker;

#endif // #ifndef HEADER_CHECKER_HPP
