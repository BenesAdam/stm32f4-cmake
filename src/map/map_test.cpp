#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "map.hpp"
#include "const_size_allocator.hpp"

void Test_cConstSizeAllocator(void)
{
	printf("Test cConstSizeAllocator\n");

	cConstSizeAllocator<uint16_t, 3U> allocator;

	uint16_t* x1 = nullptr;
	uint16_t* x2 = nullptr;
	uint16_t* x3 = nullptr;
	uint16_t* x4 = nullptr;

	allocator.Allocate(x1);
	assert((uintptr_t)(x1) == (uintptr_t)(&allocator));

	allocator.Allocate(x2);
	allocator.Allocate(x3);

	*x1 = 1U;
	*x2 = 2U;
	*x3 = 3U;

	allocator.Deallocate(x1);
	allocator.Allocate(x4);
	assert((uintptr_t)(x4) == (uintptr_t)(&allocator));

	*x4 = 4U;

	allocator.Deallocate(x2);
	allocator.Deallocate(x3);
	allocator.Deallocate(x4);

	allocator.Allocate(x1);
	assert((uintptr_t)(x1) == (uintptr_t)(&allocator));

	allocator.Allocate(x2);
	allocator.Allocate(x3);

	*x1 = 10U;
	*x2 = 20U;
	*x3 = 30U;

	allocator.Allocate(x4);
	assert(x4 == nullptr);
}

void Test_cRedBlackTree(void)
{
	printf("Test Test_cRedBlackTree\n");

	uint32_t value = 0U;
	bool founded = false;
	nsRedBlackTree::cRedBlackTree<uint16_t, uint32_t, 20U> tree;

	// Build training tree
	tree.Insert(4);
	tree.Insert(2);
	tree.Insert(9);

	tree.Insert(3);

	tree.Insert(7);
	tree.Insert(15);

	tree.Insert(18);
	tree.Insert(12);

	// Insert 20
	tree.Insert(20, 220);

	founded = tree.Search(20, value);
	assert(founded == true);
	assert(value == 220);

	value = 0;
	founded = tree.Search(999, value);
	assert(founded == false);
	assert(value == 0);

	// Build another training tree
	tree.Clean();

	tree.Insert(12);
	tree.Insert(4);
	tree.Insert(18);

	tree.Insert(2);
	tree.Insert(7);
	tree.Insert(15);

	tree.Insert(1);
	tree.Insert(5);
	tree.Insert(8);

	// Insert 10
	tree.Insert(10, 55);

	founded = tree.Search(10, value);
	assert(founded == true);
	assert(value == 55);

	printf("");
}

int main()
{
	Test_cConstSizeAllocator();
	Test_cRedBlackTree();

	printf("");
}
