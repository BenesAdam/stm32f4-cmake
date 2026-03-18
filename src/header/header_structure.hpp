#ifndef HEADER_STRUCTURE_HPP
#define HEADER_STRUCTURE_HPP

#include "types.h"

struct sHeader
{
    ui32 validationNumber;
    ui8 reserved[12U];
    ui32 startAddress;
    ui32 endAddress;
    ui8 reserved_2[8U];
    ui8 signature[256U];
    ui8 reserved_3[16U];
    ui32 exponent;
    ui8 reserved_4[12U];
    ui8 modulus[256U];
    ui8 reserved_5[448U];
};

struct sAddressRange
{
    const ui32 lowerAddress;
    const ui32 upperAddress;
};

struct sCheckMemory
{
    const sAddressRange AddressRange[5U];
};

extern sCheckMemory CheckMemory;

#endif // #ifndef HEADER_STRUCTURE_HPP
