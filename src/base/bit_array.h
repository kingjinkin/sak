/*
 * bit_array.h
 *
 *  Created on: 2013-2-26
 *      Author: joey
 */

#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_

#include <stdlib.h>
#include <string.h>
#include "bit.h"

#define ALIGN_UP(a, b) ((a + b) & (~(b-1)))
#define N_TO_BITS(n) (sizeof(bit_array_t) + ALIGN_UP(n, BITS_PER_LONG) / BITS_PER_BYTE )

typedef struct bit_unit
{
    ulong   unit;
} bit_unit_t;

typedef struct bit_array
{
    uint            size;
    bit_unit_t          bits[0];
} bit_array_t;

static inline bit_array_t* bit_array_create(uint num)
{
    bit_array_t* arr = (bit_array_t*)malloc(N_TO_BITS(num));
    if (arr)
    {
        memset(arr, 0, N_TO_BITS(num));
        arr->size = num;
//        printf("num size %d %d %d\n", num, N_TO_BITS(num), arr->bits[0].unit);
    }
    return arr;
}

static inline void bit_array_destroy(bit_array_t* arr)
{
    if (arr)
    {
        free(arr);
    }
}

static inline void bit_array_set(bit_array_t* arr, uint i)
{
    if (arr && arr->size > i)
    {
        BIT_SET(arr->bits[i / BITS_PER_LONG].unit, i % BITS_PER_LONG);
    }
}

static inline void bit_array_clear(bit_array_t* arr, uint i)
{
    if (arr && arr->size > i)
    {
        BIT_CLEAR(arr->bits[i / BITS_PER_LONG].unit, i % BITS_PER_LONG);
    }
}

static inline void bit_array_change(bit_array_t* arr, uint i)
{
    if (arr && arr->size > i)
    {
        BIT_CHANGE(arr->bits[i / BITS_PER_LONG].unit, i % BITS_PER_LONG);
    }
}

static inline int bit_array_test(bit_array_t* arr, uint i)
{
    if (arr && arr->size > i)
    {
        return bit_test(arr->bits[i / BITS_PER_LONG].unit, i % BITS_PER_LONG);
    }
    return 0;
}

static inline int bit_array_findnext(bit_array_t* arr, uint i)
{
    ulong mask = 1 << (i % BITS_PER_LONG);
    mask = ~(mask - 1);
    int j,k;
    if (arr && i < arr->size) {
        ulong tmp = arr->bits[i / BITS_PER_LONG].unit & mask;
        if (tmp)
            return i + bit_first_one(tmp);
        k = 0;
        for (j = ALIGN_UP(i,BITS_PER_LONG); j < arr->size; j += BITS_PER_LONG) {
            tmp = arr->bits[j / BITS_PER_LONG].unit;
            if(tmp) {
                k = bit_first_one(tmp);
                break;
            }
        }
        return j + k;
    }
    return -1;
}

#endif /* BIT_ARRAY_H_ */
