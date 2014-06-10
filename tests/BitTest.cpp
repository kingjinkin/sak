/*
 * TestBits.cpp
 *
 *  Created on: 2013-2-27
 *      Author: joey
 */


#include <gtest/gtest.h>
#include "bit_array.h"
#include <stdio.h>

TEST(BitTest, TestArray) {
	const int num = 1025;
	bit_array_t* bits = bit_array_create(num);
	EXPECT_TRUE(bits);

	for (uint i = 0; i < num; i++)
	{
		bit_array_set(bits, i);
		EXPECT_TRUE(bit_array_test(bits, i));

		bit_array_change(bits, i);
		EXPECT_TRUE(!bit_array_test(bits, i));

		bit_array_change(bits, i);
		EXPECT_TRUE(bit_array_test(bits, i));

		EXPECT_EQ(bit_array_findnext(bits, 0), i);

		bit_array_clear(bits, i);
		EXPECT_FALSE(bit_array_test(bits, i));
	}

	bit_array_destroy(bits);
}

TEST(BitTest, TestFind) {
    ulong a = 0xfffffff;
    EXPECT_EQ(bit_first_one(a), 27);
    for (int i = 0; i < BITS_PER_LONG; i++) {
        a = 1UL << (i);

        EXPECT_EQ(bit_first_one(a), i)<<std::hex<<"a:0x"<<a<<" i:"<<std::dec<<i;;
    }

}

