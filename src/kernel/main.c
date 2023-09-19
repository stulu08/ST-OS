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
struct MemInfo{
    uint64_t lowerSize;
    uint64_t upperSize;
};
// https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
void ParseMBI(uint32_t magicNum, uint32_t multiBootAddr, struct MBI* dest){
    dest->tag = NULL;
    dest->magic = magicNum;
    dest->addr = (uint64_t)multiBootAddr;
    dest->size = *(uint32_t*)(dest->addr);
    print("Announced mbi size: 0x");
    print_uint32(dest->size, HEX);
    print("\n");
    
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
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                print("Command line = ");
                print(((struct multiboot_tag_string *) dest->tag)->string);
                print("\n");
                break;

            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                print("Boot loader name = ");
                print(((struct multiboot_tag_string *) dest->tag)->string);
                print("\n");
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                {
                    multiboot_memory_map_t *mmap;

                    print("mmap:\n");
                    
                    for (mmap = ((struct multiboot_tag_mmap *) dest->tag)->entries;
                        (multiboot_uint8_t *) mmap 
                        < (multiboot_uint8_t *) dest->tag + dest->tag->size;
                        mmap = (multiboot_memory_map_t *) 
                        ((unsigned long) mmap
                            + ((struct multiboot_tag_mmap *) dest->tag)->entry_size)){
                        print("   base_addr: 0x");
                        print_uint64(mmap->addr, HEX);
                        print(", length: 0x");
                        print_uint64(mmap->len, HEX);
                        print(", type: 0x");
                        print_uint32(mmap->type, HEX);
                        print("\n");
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
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                dest->lowerSize = (uint64_t)KB_TO_B(((struct multiboot_tag_basic_meminfo *) mbi->tag)->mem_lower);
                dest->upperSize = (uint64_t)KB_TO_B(((struct multiboot_tag_basic_meminfo *) mbi->tag)->mem_upper);
                return;
            default:
                break;
        }
        mbi->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mbi->tag + ((mbi->tag->size + 7) & ~7));
    }
}

void MallocTest();

void kernel_main(uint32_t magic, uint32_t multiBootAddr){
    cls();
    struct MBI mbi;
    struct MemInfo info;
    ParseMBI(magic, multiBootAddr, &mbi);
    GetMemInfo(&mbi, &info);

    print("mem_lower = 0x");
    print_uint64(info.lowerSize, HEX);
    print(", mem_upper = 0x");
    print_uint64(info.upperSize, HEX);
    print("\n");


    SetupKernelHeap(info.upperSize/2);

    MallocTest();

    return;
}


void MemTest(){
    char* buf[10];
    for(int i = 0; i < 10; i++){
        buf[i] = (char*)AllocatePage();
        print("\nAllocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    for(int i = 0; i < 10; i+=2){
        FreePage((struct Page*)buf[i]);
        print("\nDeallocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    for(int i = 0; i < 12; i+=2){
        buf[i] = (char*)AllocatePage();
        print("\nAllocated at: 0x");
        print_uint32((uint32_t)((size_t)buf[i]), HEX);
    }
    return;
}
void MallocTest(){
    void* data = malloc(4097);
    print("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data), HEX);

    void* data1 = malloc(65);
    print("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data1), HEX);

    free(data);

    void* data2 = malloc(650);
    print("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data2), HEX);
    void* data3 = malloc(650);
    print("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data3), HEX);
    void* data4 = malloc(650);
    print("\nAllocated at: 0x");
    print_uint32((uint32_t)((size_t)data4), HEX);
}