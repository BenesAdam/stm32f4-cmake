#ifndef BITSET_HPP
#define BITSET_HPP

#include "types.h"

template <ui64 BitsCount, typename T = ui32>
class cBitset
{
public:
  cBitset(void);
  cBitset(const ui64 arg_number);

  bool IsSet(const ui64 arg_index) const;
  void Set(const ui64 arg_index);
  void Set(void);
  void Reset(const ui64 arg_index);
  void Reset(void);
  void Flip(const ui64 arg_index);

  ui64 Count(void) const;
  bool Any(void) const;
  bool None(void) const;
  bool All(void) const;

  ui64 Size(void) const;
  void SetBits(const ui64 arg_number);
  ui64 ToNumber(void) const;
  constexpr ui64 GetStringSize(void) const;
  void ToString(ui8* buffer) const;

  bool operator[](const ui64 arg_index) const;

private:
  struct sPosition
  {
    ui64 BlockIndex;
    T BitMask;
  };

  static constexpr ui64 GetRequiredDataSize(void);
  bool IsOutOfBounds(const ui64 arg_index) const;
  sPosition GetPosition(const ui64 arg_index) const;

  static constexpr ui8 blockSize = sizeof(T) * 8U;
  static constexpr ui64 dataSize = GetRequiredDataSize();
  T data[dataSize];
  ui64 bitsSet;
};

// Constructor.
template <ui64 BitsCount, typename T>
inline cBitset<BitsCount, T>::cBitset(void) : data{ static_cast<T>(0) }, bitsSet(0ULL)
{
}

// Constructor with number.
template<ui64 BitsCount, typename T>
inline cBitset<BitsCount, T>::cBitset(const ui64 arg_number) : cBitset<BitsCount, T>()
{
  SetBits(arg_number);
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

// Returns position of desired bit.
template<ui64 BitsCount, typename T>
inline typename cBitset<BitsCount, T>::sPosition cBitset<BitsCount, T>::GetPosition(const ui64 arg_index) const
{
  const T blockIndex = (dataSize - static_cast<T>(1)) - (arg_index / cBitset::blockSize);
  const T bitIndex = arg_index % cBitset::blockSize;
  const T mask = static_cast<T>(1) << bitIndex;

  return { blockIndex, mask };
}

// Returns true if bit is set.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::IsSet(const ui64 arg_index) const
{
  if (IsOutOfBounds(arg_index))
  {
    return false;
  }

  const sPosition position = GetPosition(arg_index);
  const ui64 maskedBlock = data[position.BlockIndex] & position.BitMask;

  return maskedBlock != static_cast<T>(0);
}

// Set desired bit.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::Set(const ui64 arg_index)
{
  if (IsOutOfBounds(arg_index))
  {
    return;
  }

  const sPosition position = GetPosition(arg_index);
  T& block = data[position.BlockIndex];

  if ((block & position.BitMask) == 0U)
  {
    bitsSet++;
    block |= position.BitMask;
  }
}

// Set all bits
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::Set(void)
{
  for (ui64 i = 0U; i < dataSize; i++)
  {
    data[i] = static_cast<T>(-1);
  }

  bitsSet = BitsCount;
}

// Reset desired bit.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::Reset(const ui64 arg_index)
{
  if (IsOutOfBounds(arg_index))
  {
    return;
  }

  const sPosition position = GetPosition(arg_index);
  T& block = data[position.BlockIndex];

  if ((block & position.BitMask) != 0U)
  {
    bitsSet--;
    block &= ~position.BitMask;
  }
}

// Reset all bits.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::Reset(void)
{
  for (ui64 i = 0U; i < dataSize; i++)
  {
    data[i] = static_cast<T>(0);
  }

  bitsSet = 0;
}

// Flip desired bit.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::Flip(const ui64 arg_index)
{
  if (IsSet(arg_index))
  {
    Reset(arg_index);
  }
  else
  {
    Set(arg_index);
  }
}

// Count the number of set bits.
template<ui64 BitsCount, typename T>
inline ui64 cBitset<BitsCount, T>::Count(void) const
{
  return bitsSet;
}

// Checks if any bit is set.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::Any(void) const
{
  return bitsSet > 0ULL;
}

// Checks if none bit is set.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::None(void) const
{
  return bitsSet == 0ULL;
}

// Check if all bit is set.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::All(void) const
{
  return bitsSet == BitsCount;
}

// Returns the size of the bitset.
template<ui64 BitsCount, typename T>
inline ui64 cBitset<BitsCount, T>::Size(void) const
{
  return BitsCount;
}

// Set bits based on decimal number.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::SetBits(const ui64 arg_number)
{
  Reset();

  ui64 index = 0ULL;
  ui64 number = arg_number;

  while (number != 0ULL)
  {
    if ((number % 2ULL) != 0U)
    {
      Set(index);
    }

    number /= 2ULL;
    index++;
  }
}

// Convert bitset to decimal number.
template<ui64 BitsCount, typename T>
inline ui64 cBitset<BitsCount, T>::ToNumber(void) const
{
  ui64 number = 0ULL;
  ui64 digit = 1ULL;

  for (ui64 i = 0ULL; i < BitsCount; i++)
  {
    number += (IsSet(i)) ? digit : 0ULL;
    digit *= 2ULL;
  }

  return number;
}

// Get size of string (with string terminator).
template<ui64 BitsCount, typename T>
inline constexpr ui64 cBitset<BitsCount, T>::GetStringSize(void) const
{
  return BitsCount + 1ULL;
}

// Convert bitset to string.
template<ui64 BitsCount, typename T>
inline void cBitset<BitsCount, T>::ToString(ui8* buffer) const
{
  for (ui64 i = 0ULL; i < BitsCount; i++)
  {
    const ui64 bufferIndex = BitsCount - 1ULL - i;
    buffer[bufferIndex] = (IsSet(i)) ? '1' : '0';
  }

  buffer[BitsCount] = '\0';
}

// Overload of index operator.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::operator[](const ui64 arg_index) const
{
  return IsSet(arg_index);
}

// Returns true if index is out of boundary.
template<ui64 BitsCount, typename T>
inline bool cBitset<BitsCount, T>::IsOutOfBounds(const ui64 arg_index) const
{
  return arg_index >= BitsCount;
}

template class cBitset<16>;

#endif /* BITSET_HPP */
