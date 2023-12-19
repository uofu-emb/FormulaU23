#include <stm32_ll_rtc.h>
#include <stm32_ll_rcc.h>
#include <stm32_ll_pwr.h>
#include "timestamp.h"

void setup_RTC(void){
    //printk("\nMade it Here\n");
    //int async_pre = 127;
    //int sync_pre = 311;
    int async_pre = 124;
    //int sync_pre = 319;
    //Value for Renode
    int sync_pre = 170;

    LL_PWR_DisableBkUpAccess();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    LL_RCC_EnableRTC();

    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_EnableInitMode(RTC);
    while(!LL_RTC_IsActiveFlag_INIT(RTC)){k_msleep(500);};
    LL_RTC_SetAsynchPrescaler(RTC, async_pre);
    LL_RTC_SetSynchPrescaler(RTC, sync_pre);
    LL_RTC_TIME_SetFormat(RTC, LL_RTC_TIME_FORMAT_AM_OR_24);
    LL_RTC_DisableInitMode(RTC);
    while(LL_RTC_IsActiveFlag_RECALP(RTC)){k_msleep(500);};
    LL_RTC_CAL_SetMinus(RTC, (uint32_t)482);
    LL_RTC_CAL_SetPulse(RTC, LL_RTC_CALIB_INSERTPULSE_SET);
    LL_RTC_EnableWriteProtection(RTC);
}

uint32_t getTimeStamp(void){
    while(!LL_RTC_IsActiveFlag_RS(RTC)){k_msleep(500);};
    return LL_RTC_TIME_Get(RTC);
    //return LL_RTC_TS_GetTime(RTC);
}

void resetTime(void){
    LL_RTC_TIME_SetHour(RTC, 0);
    LL_RTC_TIME_SetMinute(RTC, 0);
    LL_RTC_TIME_SetSecond(RTC, 0);
}
