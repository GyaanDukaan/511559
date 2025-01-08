#ifndef CUSTOM_MEMORY_ALLOCATOR_H
#define CUSTOM_MEMORY_ALLOCATOR_H

#include <cstdlib>
#include <mutex>

class CustomMemoryAllocator {
public:
    CustomMemoryAllocator(size_t totalSize);
    ~CustomMemoryAllocator();

    void* allocate(size_t size);
    void deallocate(void* ptr);

private:
    struct Block {
        size_t size;
        Block* next;
    };

    char* memoryPool;
    Block* freeList;
    std::mutex allocatorMutex;
};

#endif // CUSTOM_MEMORY_ALLOCATOR_H