#include <MEM.h>

static void* AllocatorAllocForExpHeap_(MEMAllocator* allocator, u32 size) {
    return MEMAllocFromExpHeapEx(allocator->heap, size, allocator->align);
}

static void AllocatorFreeForExpHeap_(MEMAllocator* allocator, void* block) {
    MEMFreeToExpHeap(allocator->heap, block);
}

static void* AllocatorAllocForFrmHeap_(MEMAllocator* allocator, u32 size) {
    return MEMAllocFromFrmHeapEx(allocator->heap, size, allocator->align);
}

static void AllocatorFreeForFrmHeap_(MEMAllocator* allocator, void* block) {
#pragma unused(allocator)
#pragma unused(block)
}

void* MEMAllocFromAllocator(MEMAllocator* allocator, u32 size) {
    return allocator->funcs->allocFunc(allocator, size);
}

void MEMFreeToAllocator(MEMAllocator* allocator, void* block) {
    allocator->funcs->freeFunc(allocator, block);
}

void MEMInitAllocatorForExpHeap(MEMAllocator* allocator, MEMiHeapHead* heap,
                                s32 align) {
    static MEMAllocatorFuncs sAllocatorFunc = {AllocatorAllocForExpHeap_,
                                               AllocatorFreeForExpHeap_};
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heap;
    allocator->align = align;
    allocator->WORD_0xC = 0;
}

void MEMInitAllocatorForFrmHeap(MEMAllocator* allocator, MEMiHeapHead* heap,
                                s32 align) {
    static MEMAllocatorFuncs sAllocatorFunc = {AllocatorAllocForFrmHeap_,
                                               AllocatorFreeForFrmHeap_};
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heap;
    allocator->align = align;
    allocator->WORD_0xC = 0;
}
