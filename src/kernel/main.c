#include <grub/multiboot2.h>

#include "out.h"
#include "kernel_heap.h"

void kernel_main(uint32_t magic, uint32_t multiBootAddr){
    struct multiboot_tag *tag;
    uint64_t addr = (uint64_t)multiBootAddr;
    uint32_t size = *(uint32_t*)(addr);

    cls();
    // already checked magic number in asm but why not
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC){
        kernel_panic("Magic number invalid!");
        return;
    }
    if (addr & 7) {
        kernel_panic("Unaligned mbi");
        return;
    }

    print("Announced mbi size: ");
    print_int(size);
    print("\n");


    for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                print("Command line = ");
                print(((struct multiboot_tag_string *) tag)->string);
                print("\n");
                break;

            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                print("Boot loader name = ");
                print(((struct multiboot_tag_string *) tag)->string);
                print("\n");
                break;

            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                print("mem_lower = ");
                print_int(((struct multiboot_tag_basic_meminfo *) tag)->mem_lower);
                print("KB, mem_upper = ");
                print_int(((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                print("KB\n");
                break;
        }
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                    + ((tag->size + 7) & ~7));
    }    
}