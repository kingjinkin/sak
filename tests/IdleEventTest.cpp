/*
 * IdleEventTest.cpp
 *
 *  Created on: 2013-3-3
 *      Author: joey
 */

#include <gtest/gtest.h>

extern "C" void idle_event_test();

TEST(IdleEventTest, testIdleEvent) {
    idle_event_test();
}


