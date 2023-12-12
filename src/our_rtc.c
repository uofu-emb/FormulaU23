#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/gpio.h>
#include <stdio.h>

#include "timestamp.h"


void rtc_main2(void){
    uint32_t test_time = 0;

    setup_RTC();

    resetTime();

    k_sleep(K_MSEC(5050));

    test_time = getTimeStamp();

}