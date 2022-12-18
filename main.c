#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "main.h"
#include "tuple.h"

#define DEBUG

uint16_t* ROM;


uint16_t ROM_size=0xFFFF;

int main() {
    size_t* length;
    uint8_t* data;
    uint16_t id;

    id=10;

    printf("Hello, World!\n");
    ROM=xmalloc(ROM_size);
#ifdef DEBUG
    printf("ROM Address: %p\n", ROM);
#endif

    ROM_init(ROM, ROM_size);
    ROM_fill_random(ROM, ROM_size);

    data=(uint8_t *)find_tuple(ROM, id, length);
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