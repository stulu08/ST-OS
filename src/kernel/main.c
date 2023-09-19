#include <grub/multiboot2.h>

#include "out.h"
#include "kernel_heap.h"

struct MBI{
    uint32_t magic;
    uint64_t addr;

    uint32_t size;
    struct multiboot_tag* tag;
};
struct MemInfo{
    uint32_t lowerSize;
    uint32_t upperSize;
};


void ParseMBI(uint32_t magicNum, uint32_t multiBootAddr, struct MBI* dest){
    dest->tag = NULL;
    dest->magic = magicNum;
    dest->addr = (uint64_t)multiBootAddr;
    dest->size = *(uint32_t*)(dest->addr);
    print("Announced mbi size: ");
    print_uint(dest->size);
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

            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                print("mem_lower = ");
                print_int(((struct multiboot_tag_basic_meminfo *) dest->tag)->mem_lower);
                print("KB, mem_upper = ");
                print_int(((struct multiboot_tag_basic_meminfo *) dest->tag)->mem_upper);
                print("KB\n");
                SetupKernelHeap(((struct multiboot_tag_basic_meminfo *) dest->tag)->mem_upper);
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
                                       + ((mbi->tag->size + 7) & ~7)))
    {
        switch (mbi->tag->type) {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                dest->lowerSize = ((struct multiboot_tag_basic_meminfo *) mbi->tag)->mem_lower;
                dest->upperSize = ((struct multiboot_tag_basic_meminfo *) mbi->tag)->mem_upper;
                break;
        }
        mbi->tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mbi->tag 
                                    + ((mbi->tag->size + 7) & ~7));
                                    
    }  
    
}


void kernel_main(uint32_t magic, uint32_t multiBootAddr){
    cls();
    struct MBI mbi;
    struct MemInfo info;
    ParseMBI(magic, multiBootAddr, &mbi);
    GetMemInfo(&mbi, &info);


    SetupKernelHeap(info.lowerSize+1);

    struct Page* page = AllocateNewPage();
    struct Page* page2 = AllocateNewPage();


    uint64_t address = (uint64_t)page;
    uint64_t address2 = (uint64_t)page2;

    print_uint((uint32_t)address);
    print("\n");
    print_uint((uint32_t)address2);

}