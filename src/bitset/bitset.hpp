#ifndef BITSET_HPP
#define BITSET_HPP

#include "types.h"

// Interface https://www.geeksforgeeks.org/cpp-bitset-and-its-application/

template <ui64 BitsCount, typename T = ui32>
class cBitset
{
public:
  cBitset(void);

private:
  static constexpr ui8 blockSize = sizeof(T) * 8U;
  static constexpr ui64 GetRequiredDataSize(void);

  static constexpr ui64 dataSize = GetRequiredDataSize();
  T data[dataSize];
};

template <ui64 BitsCount, typename T>
inline cBitset<BitsCount, T>::cBitset(void) :
  data{ static_cast<T>(0) }
{
}

// Returns number of blocks that is needed to store all bits.
template <ui64 BitsCount, typename T>
inline constexpr ui64 cBitset<BitsCount, T>::GetRequiredDataSize(void)
{
  constexpr ui64 remainder = BitsCount % cBitset::blockSize;
  constexpr ui64 padding = (remainder == 0U) ? 0ULL : cBitset::blockSize;
  constexpr ui64 bitsRoundedUp = BitsCount + padding;

  return bitsRoundedUp / cBitset::blockSize;
}

template class cBitset<16>;

#endif /* BITSET_HPP */
