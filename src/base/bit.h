/*
 * bit.h
 *
 *  Created on: 2013-2-27
 *      Author: joey
 */

#ifndef BIT_H_
#define BIT_H_

typedef unsigned long ulong;
typedef unsigned int uint;

#define BYTE_PER_LONG (8)
#define BITS_PER_BYTE (8)

#if !defined BITS_PER_LONG  // could be defined using -DBITS_PER_LONG=value
#include <climits>

#if  ( 4294967295UL==ULONG_MAX )  // long is 32bit
#  define  BITS_PER_LONG  32
#  define  LOG2_BITS_PER_LONG  5
#else
#  if  ( 18446744073709551615UL==ULONG_MAX )  // long is 64 bit
#    define  BITS_PER_LONG  64
#    define  LOG2_BITS_PER_LONG  6
#  else
#    define  BITS_PER_LONG  128  // ... assume long is 128 bit
#    define  LOG2_BITS_PER_LONG  7
#    error 'several functions are not implemented for 128bit long'
// contact me and I might fix it
#  endif  //   ( 18446744073709551615UL==ULONG_MAX )
#endif  //   ( 4294967295UL==ULONG_MAX )

#endif  //  !defined BITS_PER_LONG

#define BIT_SET(a, i) bit_set(&a, i)
#define BIT_CLEAR(a, i) bit_clear(&a, i)
#define BIT_CHANGE(a, i) bit_change(&a, i)
#define BIT_TEST(a, i) bit_test(a, i)

static inline void bit_set(ulong* a, uint i)
{
    *a =  *a | (1UL<<i);
}

static inline void bit_clear(ulong* a, uint i)
{
    *a =  *a & ~(1UL<<i);
}

static inline void bit_change(ulong* a, uint i)
{
    *a =  *a ^ (1UL<<i);
}

static inline int bit_test(ulong a, uint i)
{
    return (a & (1UL<<i)) != 0;
}

static inline int bit_first_one(ulong a)
{

    int i = 0;
#if BITS_PER_LONG == 64
    if (a & 0xffffffff00000000UL) {a >>= 32; i += 32;}
#endif
    if (a & 0xffff0000UL) {a >>= 16; i += 16;}
    if (a & 0x0000ff00UL) {a >>= 8; i += 8;}
    if (a & 0x000000f0UL) {a >>= 4; i += 4;}
    if (a & 0x0000000cUL) {a >>= 2; i += 2;}
    if (a & 0x00000002UL) { i += 1;}
    return i;
}

#endif /* BIT_H_ */
