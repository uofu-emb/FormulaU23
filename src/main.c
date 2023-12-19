#include <zephyr.h>
#include <stdio.h>
#include <sys/printk.h>

#include <kernel.h>
#include "i2c_driver.h"
#include "BNO055_Drv.h"

extern void send_main(void);
extern void receive_main(void);

extern void rtc_main(void);

extern void slave_setup_main(void);
extern void i2c_master_main(void);

void main(void)
{
    #ifdef SEND_ACTIVITY
        send_main();
    #endif

    #ifdef RECEIVE_ACTIVITY
        receive_main();
    #endif

    #ifdef RTC_TEST
        rtc_main();

    #ifdef SLAVE_ACTIVITY
        slave_setup_main();
    #endif

    #ifdef I2C_MASTER_ACTIVITY
        i2c_master_main();
    #endif
  
    BNO055_Begin();
    BNO055_GetVector(VECTOR_ACCELEROMETER);

    while (1)
    {
        /* code */
    }

}