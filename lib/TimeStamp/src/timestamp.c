#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stm32_ll_rtc.h>
#include "timestamp.h"

#define RTC_DEV DT_NODELABEL(rtc)

#if !DT_NODE_HAS_STATUS(RTC_DEV, okay)
#error "missing counter"
#endif

void setup_RTC(void){
    printk("\nMade it Here\n");
    int async_pre = 0x07;
    int sync_pre = 0x0007;
    
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_EnableInitMode(RTC);
    while(!LL_RTC_IsActiveFlag_INIT(RTC));
    LL_RTC_SetAsynchPrescaler(RTC, async_pre);
    LL_RTC_SetSynchPrescaler(RTC, sync_pre);
    LL_RTC_TIME_SetFormat(RTC, LL_RTC_TIME_FORMAT_AM_OR_24);
    LL_RTC_DisableInitMode(RTC);
    LL_RTC_EnableWriteProtection(RTC);

}

uint32_t getTimeStamp(void){
    return LL_RTC_TIME_Get(RTC);
}

void resetTime(void){
    LL_RTC_TIME_SetHour(RTC, 0);
    LL_RTC_TIME_SetMinute(RTC, 0);
    LL_RTC_TIME_SetSecond(RTC, 0);
}