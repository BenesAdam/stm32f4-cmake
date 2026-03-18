#ifndef HEADER_CHECKER_HPP
#define HEADER_CHECKER_HPP

#include "types.h"
#include "header_structure.hpp"

class cHeaderChecker
{
public:
    cHeaderChecker(void);
    bool FindApplication(void);
    bool CheckApplicationHeader(void);
    static void RunApplication(void) __attribute__((noreturn));

private:
    static ui32 HeaderAddress;
    sHeader* Header;
    ui8 Hash[32U];
    ui8 Hash_from_sig[32U];
};

extern cHeaderChecker HeaderChecker;


#endif // #ifndef HEADER_CHECKER_HPP
