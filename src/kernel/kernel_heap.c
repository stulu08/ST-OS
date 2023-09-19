#include "kernel_heap.h"

typedef struct Page Page;

Page* begin = NULL;
Page* end = NULL;
Page** freedPages = NULL;

void SetupKernelHeap(size_t beginLower){
    begin = end = (Page*)beginLower;
    freedPages = (Page**)AllocateNewPage();
    freedPages[0] = NULL;
}

void FreePage(Page* page){
    size_t i = 0;
    while(freedPages[i] != NULL){
        i++;
    }
    freedPages[i] = page;
}

Page* AllocateNewPage(){
    Page* page = (Page*)end;
    end += HEAP_PAGE_SIZE;
    return page;
}

Page* AllocateFreePage(){
    if(freedPages[0] == NULL)
        return NULL;

    size_t i = 0;
    while(freedPages[i] != NULL){
        i++;
    }
    Page* page = freedPages[++i];
    freedPages[i] = NULL;
    return page;
}

Page* AllocatePage() {
   Page* page = AllocateFreePage();
   if(page != NULL)
        return page;
    return AllocateNewPage();
}