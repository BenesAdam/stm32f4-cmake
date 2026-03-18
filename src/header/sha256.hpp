#ifndef SHA256_HPP
#define SHA256_HPP

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

void sha256_mem(ui32 start_addr, ui32 end_addr, ui8 hash[32]);

#endif // #ifndef SHA256_HPP
