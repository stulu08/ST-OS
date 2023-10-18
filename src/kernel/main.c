#include <grub/multiboot2.h>


#include "out.h"
#include "kernel_heap.h"
#include "mem/malloc.h"

void MemTest();

struct MBI{
    uint32_t magic;
    uint64_t addr;

    uint32_t size;
    struct multiboot_tag* tag;
};
#define KB_TO_B(KB) (KB*1024)
struct MemInfo {
    uint64_t lowerSize;
    uint64_t upperSize;

    uint64_t lowerBegin;
    uint64_t upperBegin;
};
// https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
void ParseMBI(uint32_t magicNum, uint32_t multiBootAddr, struct MBI* dest){
    dest->tag = NULL;
    dest->magic = magicNum;
    dest->addr = (uint64_t)multiBootAddr;
    dest->size = *(uint32_t*)(dest->addr);
    puts("Announced mbi size: 0x");
    print_uint32(dest->size, HEX);
    puts("\n");
    // already checked magic number in asm but why not
    if(dest->magic != MULTIBOOT2_BOOTLOADER_MAGIC){
        kernel_panic("Magic number invalid!");
        return;
    }
    if (dest->addr & 7) {
        kernel_panic("Unaligned mbi");
        return;
    }
    for (dest->tag = (struct multiboot_tag *) (dest->addr + 8);
       dest->tag->type != MULTIBOOT_TAG_TYPE_END;
       dest->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) dest->tag 
                                       + ((dest->tag->size + 7) & ~7)))
    {
        switch (dest->tag->type) {
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                puts("Boot loader name = ");
                puts(((struct multiboot_tag_string *) dest->tag)->string);
                puts("\n");
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                {
                    multiboot_memory_map_t *mmap;

                    puts("mmap:\n");
                    for (mmap = ((struct multiboot_tag_mmap *) dest->tag)->entries;
                        (multiboot_uint8_t *) mmap 
                        < (multiboot_uint8_t *) dest->tag + dest->tag->size;
                        mmap = (multiboot_memory_map_t *) 
                        ((unsigned long) mmap
                            + ((struct multiboot_tag_mmap *) dest->tag)->entry_size)){
                        puts("   base_addr: 0x");
                        print_uint64(mmap->addr, HEX);
                        puts(", length: 0x");
                        print_uint64(mmap->len, HEX);
                        puts(", type: 0x");
                        print_uint32(mmap->type, HEX);
                        puts("\n");
                    }
                }
                break;
        }
        dest->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) dest->tag 
                                    + ((dest->tag->size + 7) & ~7));
    }  
}
void GetMemInfo(struct MBI* mbi, struct MemInfo* dest){
    for (mbi->tag = (struct multiboot_tag *) (mbi->addr + 8);
       mbi->tag->type != MULTIBOOT_TAG_TYPE_END;
       mbi->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mbi->tag 
                                       + ((mbi->tag->size + 7) & ~7))) {
        switch (mbi->tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP:
                {
                    multiboot_memory_map_t *mmap;

                    for (mmap = ((struct multiboot_tag_mmap *) mbi->tag)->entries;
                        (multiboot_uint8_t *) mmap 
                        < (multiboot_uint8_t *) mbi->tag + mbi->tag->size;
                        mmap = (multiboot_memory_map_t *) 
                        ((unsigned long) mmap
                            + ((struct multiboot_tag_mmap *) mbi->tag)->entry_size)){

                        if(mmap->type == 1 && mmap->addr != 0){
                            dest->upperBegin = mmap->addr;
                            dest->upperSize = mmap->len;
                        }
                        else if(mmap->type == 1 && mmap->addr == 0){
                            dest->lowerBegin = mmap->addr;
                            dest->lowerSize = mmap->len;
                        }
                    }
                }
                break;
            default:
                break;
        }
        mbi->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mbi->tag + ((mbi->tag->size + 7) & ~7));
    }
    if(dest->upperSize == 0 || dest->lowerSize == 0){
        total_kernel_panic("Cant detect memory!");
    }
    if(dest->upperBegin == 0){
        dest->upperBegin = dest->upperSize / 2;
        kernel_panic("Can't detect beginning of upper memory! Assuming: 0x");
        print_uint64(dest->upperBegin, HEX);
        puts("\n");
        reset_color();
    }
}

void MallocTest();

void kernel_main(uint32_t magic, uint32_t multiBootAddr){
    cls();
    struct MBI mbi;
    struct MemInfo info;
    ParseMBI(magic, multiBootAddr, &mbi);
    GetMemInfo(&mbi, &info);
    printf("mem_lower = 0x%p -> 0x%p, mem_upper = 0x%p -> 0x%p\n", info.lowerBegin, info.lowerSize, info.upperBegin, info.upperSize);
    //SetupKernelHeap(info.upperBegin + 40000);
    //MallocTest();

    return;
}


void MemTest(){
    char* buf[10];
    for(int i = 0; i < 10; i++){
        buf[i] = (char*)AllocatePage();
        puts("\nAllocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    for(int i = 0; i < 10; i+=2){
        FreePage((struct Page*)buf[i]);
        puts("\nDeallocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    for(int i = 0; i < 12; i+=2){
        buf[i] = (char*)AllocatePage();
        puts("\nAllocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    return;
}
void MallocTest(){
    void* data = malloc(4097);
    puts("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data), HEX);

    void* data1 = malloc(65);
    puts("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data1), HEX);

    free(data);

    void* data2 = malloc(650);
    puts("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data2), HEX);
    void* data3 = malloc(650);
    puts("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data3), HEX);
    void* data4 = malloc(650);
    puts("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data4), HEX);
}