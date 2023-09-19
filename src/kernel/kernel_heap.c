#include "kernel_heap.h"
#include "out.h"

#define PAGE_PLUS_HEADER(PAGE) (Page*)(((uint8_t*)PAGE)+HEAP_PAGE_HEADER_SIZE)
#define PAGE_MINUS_HEADER(PAGE) (Page*)(((uint8_t*)PAGE)-HEAP_PAGE_HEADER_SIZE)

typedef struct Page Page;

Page* begin = NULL;
Page* end = NULL;
Page** freedPages = NULL;

void SetupKernelHeap(size_t beginLower){
    begin = end = (Page*)beginLower;
    // on page can only store 512 allocations, need to fix
    freedPages = (Page**)AllocateNewPage();
    freedPages[0] = NULL;

    print("Initilized Kernel-Heap at 0x");
    print_uint64((size_t)begin, HEX);
    print("\n");
}

void FreePage(Page* page){
    size_t i = 0;
    while(freedPages[i] != NULL){
        i++;
    }
    freedPages[i] = PAGE_MINUS_HEADER(page);
}

Page* AllocateNewPage(){
    Page* page = (Page*)end;
    end = (Page*)(((uint8_t*)end) + HEAP_PAGE_SIZE);
    return PAGE_PLUS_HEADER(page);
}

Page* AllocateFreePage(){
    if(freedPages[0] == NULL)
        return NULL;
    size_t i = 0;
    while(freedPages[i] != NULL){
        i++;
    }
    Page* page = freedPages[--i];
    freedPages[i] = NULL;
    return PAGE_PLUS_HEADER(page);
}

Page* AllocatePage() {
   Page* page = AllocateFreePage();
   if(page != NULL)
        return page;
    return AllocateNewPage();
}