#pragma once
#include <stdint.h>
#include <stddef.h>

#define HEAP_PAGE_SIZE 4096
#define HEAP_PAGE_HEADER_SIZE 0
struct Page {
    uint8_t data[HEAP_PAGE_SIZE];
};


void FreePage(struct Page* page);
void SetupKernelHeap(size_t heapBegin);
struct Page* AllocateNewPage();
struct Page* AllocateFreePage();
struct Page* AllocatePage();