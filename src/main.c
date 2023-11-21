#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>
#include <stdio.h>


CAN_DEFINE_MSGQ(bms_queue, 150);
CAN_DEFINE_MSGQ(imu_queue, 150);
CAN_DEFINE_MSGQ(mtr_queue, 150);

struct zcan_frame rx_frame;
int filter_id_imu, filter_id_bms, filter_id_mtr;
const struct device *can_dev;

const struct zcan_filter imu_filter = {
        .id_type = CAN_EXTENDED_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 1,
        .id_mask = CAN_EXT_ID_MASK
};

const struct zcan_filter bms_filter = {
        .id_type = CAN_EXTENDED_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x456,
        .rtr_mask = 1,
        .id_mask = CAN_EXT_ID_MASK
};

const struct zcan_filter mtr_filter = {
        .id_type = CAN_EXTENDED_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x789,
        .rtr_mask = 1,
        .id_mask = CAN_EXT_ID_MASK
};

void main(void) {
    can_dev = device_get_binding("CAN_1");

    // defining filter for each message queue
    filter_id_imu = can_attach_msgq(can_dev, &imu_queue, &imu_filter);
    filter_id_bms = can_attach_msgq(can_dev, &bms_queue, &bms_filter);
    filter_id_mtr = can_attach_msgq(can_dev, &mtr_queue, &mtr_filter);

    if (filter_id_imu < 0 || filter_id_bms < 0 || filter_id_mtr < 0) {
        printk("Unable to attach isr imu:[%d] bms:[%d] mtr:[%d] ", filter_id_imu,filter_id_bms,filter_id_mtr);
        return;
    }


    while (true) {
        if(k_msgq_num_used_get(&imu_queue) > 0){
            k_msgq_get(&bms_queue, &rx_frame, K_MSEC(50));
        }
        if(k_msgq_num_used_get(&bms_queue) > 0){
            k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
        }
        if(k_msgq_num_used_get(&mtr_queue) > 0){
            k_msgq_get(&mtr_queue, &rx_frame, K_MSEC(70));
        }

       
        

        //TODO Here SPI conversion here
    }
    
}



