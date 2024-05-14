#ifndef CONST_SIZE_ALLOCATOR_HPP
#define CONST_SIZE_ALLOCATOR_HPP

#include "types.h"
#include <cstring>

template<typename T, size_t MaxSize>
class cConstSizeAllocator
{
public:
	cConstSizeAllocator(void);
	void Allocate(T*& arg_memory);
	void Deallocate(T*& arg_memory);
	void Clean(void);

private:
	struct sItem
	{
		T data;
		sItem* next;
	};

	sItem data[MaxSize];
	sItem* front;

	bool IsDataPointerOkay(T* arg_memory) const;
};

template<typename T, size_t MaxSize>
inline cConstSizeAllocator<T, MaxSize>::cConstSizeAllocator(void) :
	data(),
	front(nullptr)
{
	this->Clean();
}

template<typename T, size_t MaxSize>
inline void cConstSizeAllocator<T, MaxSize>::Allocate(T* &arg_memory)
{
	if (front == nullptr)
	{
		arg_memory = nullptr;
		return;
	}

	arg_memory = &front->data;
	front = front->next;
}

template<typename T, size_t MaxSize>
inline bool cConstSizeAllocator<T, MaxSize>::IsDataPointerOkay(T* arg_memory) const
{
	// Check null pointer
	if (arg_memory == nullptr)
	{
		return false;
	}

	// Check array boundary
	const uintptr_t address = reinterpret_cast<uintptr_t>(arg_memory);
	const uintptr_t dataStart = reinterpret_cast<uintptr_t>(&this->data[0U]);
	const uintptr_t dataEnd = reinterpret_cast<uintptr_t>(&this->data[MaxSize]); // intentional overflow

	return (dataStart <= address) && (address < dataEnd);
}

template<typename T, size_t MaxSize>
inline void cConstSizeAllocator<T, MaxSize>::Deallocate(T*& arg_memory)
{
	if (!IsDataPointerOkay(arg_memory))
	{
		return;
	}

	sItem* item = reinterpret_cast<sItem*>(arg_memory);
	
	// Clear data
	memset(&item->data, 0U, sizeof(T));

	// Add item to free memory queue
	item->next = this->front;
	this->front = item;

	// Set memory pointer to null pointer
	arg_memory = nullptr;
}

template<typename T, size_t MaxSize>
inline void cConstSizeAllocator<T, MaxSize>::Clean(void)
{
	this->front = &data[0U];

	for (size_t i = static_cast<size_t>(0); i < MaxSize; i++)
	{
		memset(&(this->data[i].data), 0U, sizeof(T));
	}

	for (size_t i = static_cast<size_t>(1); i < MaxSize; i++)
	{
		this->data[i - static_cast<size_t>(1)].next = &data[i];
	}

	this->data[MaxSize - static_cast<size_t>(1)].next = nullptr;
}

#endif /* CONST_SIZE_ALLOCATOR_HPP */
