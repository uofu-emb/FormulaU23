#include "bmsDatagen.h"
#include <zephyr.h>
#include <device.h>
#include <can/can_loopback.h>

void periodic_message_bms(const struct device *dev)
{
    struct zcan_frame msg = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .dlc = 8,
        .data = {1,2,3,4,5,6,7,8}
    };

    struct k_timer t;
    k_timer_init(&t, NULL, NULL);
    
    while(1){
        
        can_send(dev, &msg, K_MSEC(1), NULL, NULL);
        k_timer_start(&t, K_MSEC(8), K_NO_WAIT);
        k_timer_status_sync(&t);
    }
}