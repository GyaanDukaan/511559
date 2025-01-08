#include "CustomMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <chrono>

void testAllocator(CustomMemoryAllocator& allocator) {
    constexpr int ALLOCATIONS = 1000;
    constexpr int ALLOCATION_SIZE = 64;
    std::vector<void*> allocations;

    // Allocation phase
    for (int i = 0; i < ALLOCATIONS; ++i) {
        void* mem = allocator.allocate(ALLOCATION_SIZE);
        if (mem) {
            allocations.push_back(mem);
        }
        else {
            std::cout << "Allocation failed at " << i << std::endl;
        }
    }

    // Deallocation phase
    for (void* mem : allocations) {
        allocator.deallocate(mem);
    }
}

int main() {
    const size_t poolSize = 1024 * 1024; // 1 MB
    CustomMemoryAllocator allocator(poolSize);

    auto start = std::chrono::high_resolution_clock::now();
    testAllocator(allocator);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Allocator test completed in " << duration.count() << " seconds." << std::endl;

    return 0;
}