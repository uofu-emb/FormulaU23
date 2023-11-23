#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>
//
#define LED0_NODE DT_ALIAS(led0)
#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0    DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0    DT_GPIO_FLAGS(LED0_NODE, gpios)

#define LED1_NODE DT_ALIAS(led1)
#define LED1    DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1    DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1    DT_GPIO_FLAGS(LED1_NODE, gpios)

#define LED2_NODE DT_ALIAS(led2)
#define LED2    DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN2    DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS2    DT_GPIO_FLAGS(LED2_NODE, gpios)

#define LED3_NODE DT_ALIAS(led3)
#define LED3    DT_GPIO_LABEL(LED3_NODE, gpios)
#define PIN3    DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGS3    DT_GPIO_FLAGS(LED3_NODE, gpios)

const struct device *led_dev;
const struct device *led_dev1;
const struct device *led_dev2;
const struct device *led_dev3;

const struct zcan_filter imu_filter = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 1,
        .id_mask = CAN_STD_ID_MASK
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


struct can_msg //can data message struct for queue
{
    int32_t id;
    uint8_t data[8]; 
};



//message queue to store the incoming CAN Messages
CAN_DEFINE_MSGQ(bms_queue, 150);
CAN_DEFINE_MSGQ(imu_queue, 150);
CAN_DEFINE_MSGQ(mtr_queue, 150);

struct zcan_frame rx_frame;
struct can_frame can_frame;
int filter_id_imu;
int filter_id_bms;
int filter_id_mtr;
const struct device *can_dev;

//Used for CAN attach ISR for callback onto queue structure
void rx_callback_function(struct zcan_frame *zframe, void *arg)
{
        //gpio_pin_set(led_dev, PIN0, (int)true);//red led
        struct can_msg can_message;
        can_message.id= zframe->id;
        //gpio_pin_set(led_dev, PIN0, (int)true);//red led
        for(int i=0; i<8; i++)
        {
            gpio_pin_set(led_dev, PIN0, (int)true);//red led
            k_msleep(1000);
            gpio_pin_set(led_dev, PIN0, (int)false);//red led
            can_message.data[i]= zframe->data[i];
        }
        //gpio_pin_set(led_dev, PIN0, (int)true);//red led
        int check= k_msgq_put(&imu_queue, &can_message, K_NO_WAIT);
        
       // k_msleep(1000);
        //gpio_pin_set(led_dev, PIN0, (int)false);//red led
        if(check==-ENOMSG)
        {
            //k_msleep(150);
            gpio_pin_set(led_dev, PIN0, (int)true);//red led
            k_msleep(150);
            gpio_pin_set(led_dev, PIN0, (int)false);
            k_msleep(150); 
        }
        if(check==-EAGAIN)
        {
            gpio_pin_set(led_dev1, PIN1, (int)true);//orange led
            k_msleep(100);
            gpio_pin_set(led_dev1, PIN1, (int)false);
            k_msleep(150); 
        }
        if(check==0)
        {
            gpio_pin_set(led_dev2, PIN2, (int)true);//green led
            k_msleep(100);
            gpio_pin_set(led_dev2, PIN2, (int)false);
            k_msleep(150); 
        }
        else
        {
            gpio_pin_set(led_dev2, PIN2, (int)true);//green led
        }
        // if(check > 0)
        // {
        //     gpio_pin_set(led_dev2, PIN2, (int)true);//green led
        //     k_msleep(100);
        //     gpio_pin_set(led_dev2, PIN2, (int)false);
        //     k_msleep(150); 
        // }
        
}


void led_setup()
{
    led_dev = device_get_binding(LED0);
    led_dev1 = device_get_binding(LED1);
    led_dev2 = device_get_binding(LED2);
    led_dev3 = device_get_binding(LED3);
    gpio_pin_configure(led_dev, PIN0, GPIO_OUTPUT_INACTIVE | FLAGS0);
    gpio_pin_configure(led_dev1, PIN1, GPIO_OUTPUT_INACTIVE | FLAGS1);
    gpio_pin_configure(led_dev2, PIN2, GPIO_OUTPUT_INACTIVE | FLAGS2);
    gpio_pin_configure(led_dev3, PIN3, GPIO_OUTPUT_INACTIVE | FLAGS3);

}


void receive_main(void) {
    can_dev = device_get_binding("CAN_1");
    can_set_mode(can_dev, CAN_NORMAL_MODE);

    led_setup();


    filter_id_imu = can_attach_msgq(can_dev, &imu_queue, &imu_filter);
    filter_id_bms = can_attach_msgq(can_dev, &bms_queue, &imu_filter);
    filter_id_mtr = can_attach_msgq(can_dev, &mtr_queue, &imu_filter);

    //int callback_check = can_attach_isr(can_dev, &rx_callback_function, NULL, &imu_filter);
     

    if (filter_id_imu < 0 || filter_id_bms < 0 || filter_id_mtr < 0) {
        printk("Unable to attach isr imu:[%d] bms:[%d] mtr:[%d] ", filter_id_imu,filter_id_bms,filter_id_mtr);
        return;
    }

    while (true) {
//////
        if(filter_id_bms == bms_filter.id)
        {
            //k_msgq_get(&bms_queue, &rx_frame, K_MSEC(50));
            gpio_pin_set(led_dev, PIN0, (int)true);
        }
        // if(callback_check == CAN_NO_FREE_FILTER)
        // {
        //     //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
        //     gpio_pin_set(led_dev2, PIN2, (int)true); //green
        // }
        // if(callback_check < 0)
        // {
        //     //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
        //     gpio_pin_set(led_dev3, PIN3, (int)true); //blue
        // }
        // if(callback_check > 0)
        // {
        //     //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
        //     gpio_pin_set(led_dev3, PIN3, (int)true); //blue
        // }
        if(filter_id_imu == CAN_NO_FREE_FILTER)
        {
            //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
            gpio_pin_set(led_dev1, PIN1, (int)true); //orange
        }
        if(filter_id_imu == 0)
        {
            //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
            gpio_pin_set(led_dev1, PIN1, (int)true); //orange
        }
        if(filter_id_imu < 0)
        {
            //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
            gpio_pin_set(led_dev1, PIN1, (int)true); //orange
        }
    
        if(filter_id_bms == 1)
        {
            //k_msgq_get(&imu_queue, &rx_frame, K_MSEC(60));
            gpio_pin_set(led_dev3, PIN3, (int)true); //blue
        }
        if(filter_id_mtr == 2)
        {
            //k_msgq_get(&mtr_queue, &rx_frame, K_MSEC(70));
            gpio_pin_set(led_dev2, PIN2, (int)true); //green
        }

        //TODO Here SPI conversion here
    }
    
}



