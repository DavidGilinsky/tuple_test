//
// Created by gilinsky on 12/17/22.
//

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "tuple.h"

// #define DEBUG

// Initialize the simulated ROM
// address space by filling it with 0xFF
void ROM_init(uint16_t *ROM, size_t size)
{
    uint16_t i;
    i = 0;
    while(i<(uint16_t)size)
    {
        *((uint8_t *)ROM+i)=0xFF;
        i=i+1;
    }
}


uint16_t *find_tuple(uint16_t *rom, uint16_t id, size_t *len)
{
    assert(rom!=NULL);
    tuple_t *tuple;
    tuple=(tuple_t *)rom;

    while(tuple->ID != id)
    {
        if(tuple->next!=0xFFFF)
        {
            tuple=(tuple_t *)((uint8_t *)rom+tuple->next);
        } else return NULL;
    }
    *len=tuple->length;
    return((uint16_t *)(tuple+1));

    return(0);
}

void ROM_fill_random(uint16_t *ROM, size_t size)
{
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
    uint16_t i;
    i = 0;
    while(i<(uint16_t)size)
    {
        *((uint8_t *)ROM+i)=0xFF;
        i=i+1;
    }
}