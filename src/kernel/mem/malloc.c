#include "mem/malloc.h"

#include "kernel_heap.h"
#include "out.h"

struct Header{
    size_t size;
};
typedef struct Header Header;

#define BLOCK_PLUS_HEADER(BLOCK) (void*)(((uint8_t*)BLOCK)+sizeof(Header))
#define BLOCK_MINUS_HEADER(BLOCK) (void*)(((uint8_t*)BLOCK)-sizeof(Header))


void* malloc_bigger(size_t size){
    uint32_t count = size / HEAP_PAGE_SIZE;
    count += (size % HEAP_PAGE_SIZE) == 0 ? 0 : 1;

    Header* data = (Header*)AllocateNewPage();
    for(uint32_t i = 1; i < count; i++){
        AllocateNewPage();
    }
    data->size = HEAP_PAGE_SIZE * count;
    return data;
}

void* malloc_smaller(size_t size){
    Header* data = (Header*)AllocatePage();
    data->size = HEAP_PAGE_SIZE;
    return data;
}

void* malloc_same(){
    Header* data = (Header*)AllocatePage();
    data->size = HEAP_PAGE_SIZE;
    return data;
}

void free_bigger(Header* block){
    if(block->size % HEAP_PAGE_SIZE != 0){
        kernel_panic("Invalid block size: ");
        print_uint64(block->size, 10);
        return;
    }

    for(Header* last = (Header*)(((uint8_t*)block) + block->size - HEAP_PAGE_SIZE); last >= block; last = (Header*)(((uint8_t*)last)-HEAP_PAGE_SIZE)){
        FreePage((struct Page*)last);
    }
}

void free_smaller(Header* block){
    FreePage((struct Page*)block);
}

void free_same(Header* block){
    FreePage((struct Page*)block);
}


void* malloc(size_t size){
    size += sizeof(Header);

    if(size < HEAP_PAGE_SIZE){
        return BLOCK_PLUS_HEADER(malloc_smaller(size));
    }else if(size > HEAP_PAGE_SIZE){
        return BLOCK_PLUS_HEADER(malloc_bigger(size));
    }
    return BLOCK_PLUS_HEADER(malloc_same());
}
void free(void* _block){
    Header* block = (Header*)BLOCK_MINUS_HEADER(_block);
    if(block->size < HEAP_PAGE_SIZE){
        return free_smaller(block);
    }else if(block->size > HEAP_PAGE_SIZE){
        return free_bigger(block);
    }
    return free_same(block);
}