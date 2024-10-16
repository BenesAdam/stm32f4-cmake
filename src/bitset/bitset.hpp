#ifndef BITSET_HPP
#define BITSET_HPP

#include "types.h"

// Interface https://www.geeksforgeeks.org/cpp-bitset-and-its-application/

template <ui64 BitsCount, typename T = ui32>
class cBitset
{
public:
  cBitset(void);

  bool operator[](const ui64 arg_index) const;

private:
  struct sPosition
  {
    ui64 BlockIndex;
    T BitMask;
  };

  static constexpr ui64 GetRequiredDataSize(void);
  void SetBit(const ui64 arg_index);
  void ResetBit(const ui64 arg_index);
  bool IsBitSet(const ui64 arg_index) const;
  bool IsOutOfBounds(const ui64 arg_index) const;
  sPosition GetPosition(const ui64 arg_index) const;

  static constexpr ui8 blockSize = sizeof(T) * 8U;
  static constexpr ui64 dataSize = GetRequiredDataSize();
  T data[dataSize];
};

template <ui64 BitsCount, typename T>
inline cBitset<BitsCount, T>::cBitset(void) : data{static_cast<T>(0)}
{
}

template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::operator[](const ui64 arg_index) const
{
  return IsBitSet(arg_index);
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

// Set desired bit.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::SetBit(const ui64 arg_index)
{
  if (IsOutOfBounds(arg_index))
  {
    return;
  }

  const sPosition position = GetPosition(arg_index);
  data[position.BlockIndex] |= position.BitMask;
}

// Reset desired bit.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::ResetBit(const ui64 arg_index)
{
  if (IsOutOfBounds(arg_index))
  {
    return;
  }

  const sPosition position = GetPosition(arg_index);
  data[position.BlockIndex] &= ~position.BitMask;
}

// Returns position of desired bit.
template<ui64 BitsCount, typename T>
inline typename cBitset<BitsCount, T>::sPosition cBitset<BitsCount, T>::GetPosition(const ui64 arg_index) const
{
  const T blockIndex = arg_index / cBitset::blockSize;
  const T zeroPositionMask = static_cast<T>(1) << (cBitset::blockSize - 1);
  const T mask = zeroPositionMask >> (arg_index % cBitset::blockSize);

  return { blockIndex, mask };
}

// Returns true if bit is set.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::IsBitSet(const ui64 arg_index) const
{
  if (IsOutOfBounds(arg_index))
  {
    return false;
  }

  const sPosition position = GetPosition(arg_index);
  const ui64 maskedBlock = data[position.BlockIndex] & position.BitMask;

  return maskedBlock != static_cast<T>(0);
}

// Returns true if index is out of boundary.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::IsOutOfBounds(const ui64 arg_index) const
{
  return arg_index >= BitsCount;
}

template class cBitset<16>;

#endif /* BITSET_HPP */
