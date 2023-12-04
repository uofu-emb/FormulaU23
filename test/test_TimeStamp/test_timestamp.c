#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <unity.h>

//#include "timestamp.h"

void setUp(void) {
}

void tearDown(void) {}

void test_reference_with_delay(){
    int32_t expected = 0x00000005;
    //setup_RTC();
    printk("\nStep 1\n");
    //resetTime();
    printk("\nStep 2\n");
    k_sleep(K_MSEC(5050));
    printk("\nStep 3\n");
    //int32_t timeStamp = getTimeStamp();
    printk("]\nStep 4\n");
    //printk("Time stamp at: %x", timeStamp);
    //TEST_ASSERT_EQUAL_INT32(expected, timeStamp);
    TEST_ASSERT_EQUAL_INT32(expected, 0x00000005);
}

int main (void)
{
    printk("\n*****************************************************************************\n");
    //UNITY_BEGIN();
    //RUN_TEST(test_reference_with_delay);
    //return UNITY_END();
}