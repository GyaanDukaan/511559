#include "CustomMemoryAllocator.h"
#include <iostream>

CustomMemoryAllocator::CustomMemoryAllocator(size_t totalSize) {
    memoryPool = (char*)malloc(totalSize);
    freeList = reinterpret_cast<Block*>(memoryPool);
    freeList->size = totalSize - sizeof(Block);
    freeList->next = nullptr;
}

CustomMemoryAllocator::~CustomMemoryAllocator() {
    free(memoryPool);
}

void* CustomMemoryAllocator::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    Block* current = freeList;
    Block* previous = nullptr;

    while (current) {
        if (current->size >= size) {
            if (current->size > size + sizeof(Block)) {
                Block* newBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(current) + sizeof(Block) + size);
                newBlock->size = current->size - size - sizeof(Block);
                newBlock->next = current->next;
                current->size = size;
                current->next = newBlock;
            }
            if (previous) {
                previous->next = current->next;
            }
            else {
                freeList = current->next;
            }
            return reinterpret_cast<char*>(current) + sizeof(Block);
        }
        previous = current;
        current = current->next;
    }
    return nullptr; // Out of memory
}

void CustomMemoryAllocator::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    Block* blockToFree = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));
    blockToFree->next = freeList;
    freeList = blockToFree;
}