#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>
#include <drivers/uart.h>
//#include <string.h>
#include <stdio.h>

//LED CONFIG
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

const char * uart_name = "UART_1";
const struct device *uart_dev;
//int ret;

#define BUF_SIZE 64

uint8_t rx_buf[BUF_SIZE];
uint8_t tx_buf[] = "Hello, UART!\n";

void print_terminal(uint8_t message[], unsigned int message_size);



void uart_setup(){
    uart_dev = device_get_binding(uart_name);
    struct uart_config uart_cfg = {
        .baudrate =9600,
        .parity = UART_CFG_PARITY_NONE,
        .stop_bits = UART_CFG_STOP_BITS_1,
        .data_bits = UART_CFG_DATA_BITS_8,
    };

    int ret = uart_configure(uart_dev, &uart_cfg);
}

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

void led_on(char color)
{
    if(color == 'r')
    {
        gpio_pin_set(led_dev, PIN0, (int)true); //red
    }
    else if(color == 'o')
    {
        gpio_pin_set(led_dev1, PIN1, (int)true); //orange
    }
    else if(color == 'g')
    {
        gpio_pin_set(led_dev2, PIN2, (int)true); //green
    }
    else if(color == 'b')
    {
        gpio_pin_set(led_dev3, PIN3, (int)true); //blue
    }
    
}

void led_off(char color)
{
    if(color == 'r')
    {
        gpio_pin_set(led_dev, PIN0, (int)false); //red
    }
    else if(color == 'o')
    {
        gpio_pin_set(led_dev1, PIN1, (int)false); //orange
    }
    else if(color == 'g')
    {
        gpio_pin_set(led_dev2, PIN2, (int)false); //green
    }
    else if(color == 'b')
    {
        gpio_pin_set(led_dev3, PIN3, (int)false); //blue
    }
    
}

void receive_main(void) 
{   //can device setup
    can_dev = device_get_binding("CAN_1");
    can_set_mode(can_dev, CAN_NORMAL_MODE);

    led_setup();

    uart_setup();

    //Adding messages from CAN bus to their respective queues
    filter_id_imu = can_attach_msgq(can_dev, &imu_queue, &imu_filter);
    filter_id_bms = can_attach_msgq(can_dev, &bms_queue, &bms_filter);
    filter_id_mtr = can_attach_msgq(can_dev, &mtr_queue, &mtr_filter);

    if (filter_id_imu < 0 || filter_id_bms < 0 || filter_id_mtr < 0) {
        printk("Unable to attach isr imu:[%d] bms:[%d] mtr:[%d] ", filter_id_imu,filter_id_bms,filter_id_mtr);
        
        uint8_t error_filter[] = "unable to attach isr";
        print_terminal(error_filter, sizeof(error_filter));
        return;
    }
    uint8_t test_message[] = "Right before the while loop \n\r";
    print_terminal(test_message, sizeof(test_message));
    while (true) 
    {

        ///Check = zero for each attachment to their respective queue using can_attaach_msq from the CAN bus with messages
        if(filter_id_imu == 0)
        {
            led_on('r');
            //print_terminal(tx_buf, sizeof(tx_buf));
        }
        if(filter_id_bms == 0)
        {
            led_on('o');
        }
        if(filter_id_mtr == 0)
        {
            led_on('b');
        }

        ///TODO Here SPI conversion here
    }
}

void print_terminal(uint8_t message[], unsigned int message_size){

    for(int i = 0; i < message_size; i++)
    {
        uart_poll_out(uart_dev, message[i]);
    }
}

