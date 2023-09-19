#pragma once
#include <stdint.h>
#include <stddef.h>

#define HEAP_PAGE_SIZE 4096

struct Page {
    char data[HEAP_PAGE_SIZE];
};

void SetupKernelHeap(size_t heapBegin);
struct Page* AllocateNewPage();
struct Page* AllocateFreePage();
struct Page* AllocatePage();