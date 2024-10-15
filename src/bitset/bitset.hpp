#ifndef BITSET_HPP
#define BITSET_HPP

#include "types.h"

template <ui32 Size, typename T = ui32>
class cBitset
{
public:
	cBitset(void);

private:
	static constexpr ui32 UnitSize = sizeof(T) * 8U;
	static constexpr ui32 GetRequiredDataSize(void);

	static constexpr ui32 dataSize = GetRequiredDataSize();
	T data[dataSize];
};

template <ui32 Size, typename T>
inline cBitset<Size, T>::cBitset(void) :
	data{static_cast<T>(0)}
{
}

template <ui32 Size, typename T>
inline constexpr ui32 cBitset<Size, T>::GetRequiredDataSize(void)
{
	const ui32 missing = (cBitset::UnitSize - (Size % cBitset::UnitSize)) % cBitset::UnitSize;
	return (Size + missing) / cBitset::UnitSize;
}

template class cBitset<16>;

#endif /* BITSET_HPP */
