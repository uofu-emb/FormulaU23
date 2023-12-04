//LAB 7 - CAN Bus

#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>
#include <stdio.h>

//message -1
struct zcan_frame frame1 = 
        {
                .id_type = CAN_STANDARD_IDENTIFIER,
                .rtr = CAN_DATAFRAME,
                .id = 0x123,
                .dlc = 8,
                .data = {1,2,3,4,5,6,7,8}
        };
//message -2
struct zcan_frame frame2 = 
        {
                .id_type = CAN_STANDARD_IDENTIFIER,
                .rtr = CAN_DATAFRAME,
                .id = 0x456,
                .dlc = 8,
                .data = {1,2,3,4,5}

        };
//message -3. Motor Control: TODO: fill in the data below.
struct zcan_frame frame3MtrCntrl = 
        {
                .id_type = CAN_STANDARD_IDENTIFIER,
                .rtr = CAN_DATAFRAME,
                .id = 0x789,
                .dlc = 8,
                .data = {1,2}

        };

//filter-mask =0 (essentially ignores the id)
const struct zcan_filter all_filter =
 {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 0,
        .id_mask = CAN_STD_ID_MASK
};


//CAN messages
const struct device *can_dev1;
int ret;
int ret1;
int filter_id1;
int filter_id2;

//send a single message to bus
void send_message(struct zcan_frame *frame)
{
        ret = can_send(can_dev1, frame, K_MSEC(150), NULL, NULL); // send message 1
        if (ret != CAN_TX_OK) 
        {
            printk("Sending failed [%d]", ret);
        }         
}

//send 2 messages to bus
void send_message_2(void)
{
        ret = can_send(can_dev1, &frame1, K_MSEC(100), NULL, NULL); // send message 1
        if (ret != CAN_TX_OK) {
                printk("Sending failed [%d]", ret);
                }     
        k_sleep(K_MSEC(250)); //wait before sending again

        ret1 = can_send(can_dev1, &frame2, K_MSEC(100), NULL, NULL); //send message 2
        if (ret1 != CAN_TX_OK) {
                printk("Sending failed [%d]", ret);
                }     
}

void send_main(void)
{
        //Activity 0
        can_dev1 = device_get_binding("CAN_1");
        can_set_mode(can_dev1, CAN_NORMAL_MODE); 
  
        while(1)
        {
                send_message(&frame1); //send message 1
                //send_message(&frame2); //send message 2
                k_msleep(150);
                send_message(&frame3MtrCntrl); //send message 3
        }

}
