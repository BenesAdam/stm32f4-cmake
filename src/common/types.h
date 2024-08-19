#ifndef TYPES_HPP
#define TYPES_HPP

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Unsigned Int
typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

constexpr ui8 ui8_min = 0U;
constexpr ui8 ui8_max = UINT8_MAX;

constexpr ui16 ui16_min = 0U;
constexpr ui16 ui16_max = UINT16_MAX;

constexpr ui32 ui32_min = 0U;
constexpr ui32 ui32_max = UINT32_MAX;

constexpr ui64 ui64_min = 0U;
constexpr ui64 ui64_max = UINT64_MAX;

// signed Int
typedef int8_t si8;
typedef int16_t si16;
typedef int32_t si32;
typedef int64_t si64;

constexpr si8 si8_min = INT8_MIN;
constexpr si8 si8_max = INT8_MAX;

constexpr si16 si16_min = INT16_MIN;
constexpr si16 si16_max = INT16_MAX;

constexpr si32 si32_min = INT32_MIN;
constexpr si32 si32_max = INT32_MAX;

constexpr si64 si64_min = INT64_MIN;
constexpr si64 si64_max = INT64_MAX;

// map defines
#define NOINIT __attribute__((section(".noinit")))


#endif // TYPES_HPP
