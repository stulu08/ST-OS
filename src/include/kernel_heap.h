#pragma once
#include <stdint.h>
#include <stddef.h>

// 1.6GB
#define HEAP_BEGIN ((void*)0x60000000)
#define HEAP_PAGE_SIZE 4096

struct Page {
    char data[HEAP_PAGE_SIZE];
};

void SetupKernelHeap();
struct Page* AllocateNewPage();
struct Page* AllocateFreePage();
struct Page* AllocatePage();