#include <zephyr.h>
#include <stdio.h>
#include <sys/printk.h>
extern void send_main(void);
extern void receive_main(void);

extern void rtc_main(void);

extern void slave_setup_main(void);



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

}