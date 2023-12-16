#ifndef OUR_TIMESTAMP
#define OUR_TIMESTAMP

#include <stdint.h>

void setup_RTC(void);
uint32_t getTimeStamp(void);
void resetTime(void);
#endif