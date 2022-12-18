//
// Created by gilinsky on 12/17/22.
//

#ifndef TUPLE_TEST_TUPLE_H
#define TUPLE_TEST_TUPLE_H

#define TUPLE_MAX_PAYLOAD 128

#include <stdint.h>

typedef struct
{
    uint16_t ID;
    uint16_t next;
    uint16_t length;
} tuple_t;

uint16_t *find_tuple(uint16_t *rom, uint16_t id, size_t *len);
void ROM_init(uint16_t *ROM, size_t size);
void ROM_fill_random(uint16_t *ROM, size_t size);
void ROM_fill_zeros(uint16_t *ROM, size_t size);

#endif //TUPLE_TEST_TUPLE_H
