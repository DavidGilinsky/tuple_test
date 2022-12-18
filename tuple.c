//
// Created by gilinsky on 12/17/22.
//

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "tuple.h"

// #define DEBUG

// Initialize the simulated ROM
// address space by filling it with 0xFF
void ROM_init(uint16_t *ROM, size_t size)
{
    assert(ROM!=NULL && "ROM_init: NULL pointer: *rom");
    uint16_t i;
    i = 0;
    while(i<(uint16_t)size)
    {
        // write 0xFF into RAM
        *((uint8_t *)ROM+i)=0xFF;
        i=i+1;
    }
}

// find the tuple with the given ID, return a pointer to the data section
uint16_t *find_tuple(const uint16_t *rom, uint16_t id, size_t *len)
{
    assert(rom!=NULL && "find_tuple: NULL pointer: *rom");
    // declare a tuple_t structure to overlay on the ROM
    tuple_t *tuple;
    // use count to keep track of how many times we've iterated through the while
    // this is like a TTL failsafe. count should never exceed the size of the ROM
    uint16_t count;
    // overlay the tuple structure on the ROM
    tuple=(tuple_t *)rom;
    count=0;

    while(tuple->ID != id)
    {
        // tuple->next should never be 0. This would be an infinte loop.
        assert(tuple->next != 0x0000 && "ERROR: find_tuple: tuple->next == 0x0000 - Infinite Loop");
        // although not explicitly stated in the instructions
        // the tuples should be consecutive.
        // We're reading from ROM, no reason for the tuples to be
        // out of order.
        assert((uint64_t)rom+tuple->next > (uint64_t)tuple
               && "ERROR: find_tuple: out of order tuples detected");
        // look for the last tuple flag, 0xFFFF
        if(tuple->next!=0xFFFF)
        {
            tuple=(tuple_t *)((uint8_t *)rom+tuple->next);
            count=count+1;
            assert(count<0xFFFE && "ERROR: find_tuple: no final tuple");
        } else return NULL;
    }
    *len=tuple->length;
    // return a pointer to the first uint16_t of data
    // because tuple is a structure, incrementing the type
    // casted pointer to the structure address the size of the struct
    // to the base address.
    return((uint16_t *)(tuple+1));

    return(0);
}
// fill ROM with random tuples
// TODO: add random seed to argument list
void ROM_fill_random(uint16_t *ROM, size_t size)
{
   assert(ROM!=NULL && "ROM_fill_random: NULL pointer: *rom");
   tuple_t* header;
   uint16_t filled;
   uint16_t remaining;
   uint16_t ID;

   header=(tuple_t *)ROM;

   filled=0;
   remaining=0;
   ID=0;

   while(filled+sizeof(*header)<(uint16_t)size-1)
   {
       header=(tuple_t *)((uint8_t *)ROM+filled);
       header->ID=ID;
       if((uint16_t)size-filled > TUPLE_MAX_PAYLOAD)
       {
           header->length = rand() % TUPLE_MAX_PAYLOAD;
       } else
       {
           header->length = rand() % ((uint16_t)size-filled);
       }
       filled=filled+sizeof(*header);
       header->next=filled+header->length;
#ifdef DEBUG
       printf("Header Address:%p\n",header);
       printf("Header ID:%x\n",header->ID);
       printf("Header Length:%d\n",header->length);
       printf("Header Next:%d\t%p\n",header->next,((uint8_t *)ROM)+header->next);
#endif
       while(filled<header->next)
       {
           *((uint8_t *)ROM+filled)=0xBE;
           filled=filled+1;
#ifdef DEBUG_2
           printf("filled: %d\t %p\t %c\n",filled, (ROM+filled), *(ROM+filled));
#endif
       }
       ID=ID+1;
   }
    header->next=0xFFFF;
}

void ROM_fill_zeros(uint16_t* ROM, size_t size)
{
    assert(ROM!=NULL && "ROM_fill_zeros: NULL pointer: *rom");
    uint16_t i;
    i = 0;
    while(i<(uint16_t)size)
    {
        *((uint8_t *)ROM+i)=0x00;
        i=i+1;
    }
}

// Check the ROM for at least one tuple and a valid end flag (0xFFFF in header->next)
bool ROM_sanity_check(const uint16_t *ROM, size_t size)
{
    assert(ROM!=NULL && "ROM_sanity_check: NULL pointer: *rom");
    tuple_t* header;
    uint16_t position;
    uint16_t tupleCount;
    position=0;
    tupleCount=0;

    header=(tuple_t *)ROM;

    while(position<(uint16_t)size)
    {
        header=(tuple_t *)((uint8_t *)ROM+position);
        //check for a sane value in header->next
        if(header->next==0)
        {
#ifdef DEBUG
            printf("ROM corrupt. header->next==0\n");
#endif
            return false;
        }
        if((uint64_t)ROM+header->next <= (uint64_t)header)
        {
#ifdef DEBUG
            printf("ROM_sanity_check: tuples out of order\n");
#endif
            return false;
        }
        tupleCount=tupleCount+1;
#ifdef DEBUG
        printf("tuples: %d\n",tupleCount);
#endif
        //check if this is the last tuple
        if(header->next==0xFFFF)
        {
#ifdef DEBUG
            printf("ROM_sanity_checkl: last tuple found: %d", tupleCount);
#endif
            return true;
        }
        position=header->next;
    }

    return false;
}