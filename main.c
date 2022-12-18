#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "main.h"
#include "tuple.h"

#define DEBUG
// We need a pointer to a block of memory we can modify to simulate ROM
uint16_t* ROM;
// And we need pointer to the same block of memory declared as const
// We'll fool the compiler into allowing us to do this
const uint16_t* ROM_const;
// by copying the address of the block to a temporary variable
uint64_t rom_address;
// then assigning that address to the const block

// We'll simulate a 64K ROM
uint16_t ROM_size=0xFFFF;

int main() {
    size_t* length;
    uint8_t* data;
    uint16_t id;
// This is the tuple ID we'll search for in the find_tuple test
    id=10;

    printf("Hello, World!\n");
    // Allocate the RAM
    ROM=xmalloc(ROM_size);
    // Copy the address to our temporary variable
    rom_address=(uint64_t)ROM;
    // Now create the alias by copying the address to the pointer to const
    ROM_const=(uint16_t *)rom_address;

#ifdef DEBUG
    printf("ROM Address: %p\n", ROM);
#endif

    // Write 0xFF to the entire block
    ROM_init(ROM, ROM_size);
    // Create random tuples in the block
    ROM_fill_random(ROM, ROM_size);

    // run a sanity check on the ROM
    if(!ROM_sanity_check(ROM_const, ROM_size))
    {
        printf("ROM Corrupt");
        return 0;
    }
    // find a tuple by its ID - Pass the pointer to const
    data=(uint8_t *)find_tuple(ROM_const, id, length);
    // spit out the data block associated with the tuple
    if(data!=NULL)
    {
       printf("Found Tuple %d - length %d ", id,(uint16_t) *length);
       printf("Data:\n");
       for(uint16_t i=0; i<(uint16_t) *length;i=i+1)
        {
            printf("%d\t%p\t%x\n", i,(data+i),*(data+i));
        }
    } else printf("Tuple %d not found.\n", id);
    printf("\n");

    //find all the tuples
    id=0;
    while(find_tuple(ROM_const, id, length)!=NULL)
    {
        printf("Found tuple %d with length %d\n", id, (uint16_t )*length);
        id=id+1;
    }

    printf("Testing error handling.\n");
    printf("Filling ROM with zeros.\n");
    ROM_fill_zeros(ROM,ROM_size);
    printf("sanity check: %s\n",
           ROM_sanity_check(ROM_const, ROM_size) ? "true" : "false");

    printf("Filling ROM with random tuples\n");
    ROM_fill_random(ROM, ROM_size);
    printf("Putting bogus value in final tuple->next\n");
    *((uint8_t *)ROM+0xFFF4)=0xFE;
    *((uint8_t *)ROM+0xFFF5)=0xFF;
    printf("Running Sanity Check\n");
    printf("sanity check: %s\n",
           ROM_sanity_check(ROM_const, ROM_size) ? "true" : "false");

    printf("Setting header-next of tuple 3 to head of tuple 2.\n");
    *((uint8_t *)ROM+0x00CF)=0x67;
    *((uint8_t *)ROM+0x00D0)=0x00;

    printf("Running find_touple.\n");
    if(find_tuple(ROM_const, id, length)==NULL)
    {
        printf("Expected result - NULL return with 0x0000 as header->next.\n");
    } else
    {
        printf("Not the expected results. Non NULL return.\n");
    }
    free(ROM);
    return 0;
}

void* xmalloc(size_t size)
{
    void* value=malloc(size);
    if(value==0)
    {
        assert ("Out of memory!");
    }
    return value;
}