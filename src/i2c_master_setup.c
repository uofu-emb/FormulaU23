#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>

/*Status LED DT reference*/
#define LED0_NODE DT_ALIAS(led2)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0 DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0 DT_GPIO_FLAGS(LED0_NODE, gpios)

/*Error LED DT reference*/
#define LED1_NODE DT_ALIAS(led3)
#define LED1 DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1 DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1 DT_GPIO_FLAGS(LED1_NODE, gpios)

/*I2C DT reference*/
#define I2C_LABEL DT_PROP(DT_NODELABEL(i2c1), label)

void i2c_master_main(void){
    /*Instantiate status LED; Initialize to ON*/
    const struct device *dev_led_status;
    dev_led_status = device_get_binding(LED0);
    int ret_led_status = gpio_pin_configure(dev_led_status, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
    gpio_pin_set(dev_led_status, PIN0, (int)true);
    k_msleep(1000);

    /*Instantiate error LED; Initialize to OFF*/
    const struct device *dev_led_error;
    dev_led_error = device_get_binding(LED1);
    int ret_led_error = gpio_pin_configure(dev_led_error, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
    gpio_pin_set(dev_led_error, PIN1, (int)false);
    k_msleep(1000);
    
    /*Instantiate I2C peripheral in Master configuration*/
    const struct device *dev_i2c;
    dev_i2c = device_get_binding(I2C_LABEL);
    if(!device_is_ready(dev_i2c))
    {
        /*Turn off status LED if I2C is not ready*/
        gpio_pin_set(dev_led_status, PIN0, (int)false);
        k_msleep(500);
    }
    uint32_t i2c_config = I2C_SPEED_SET(I2C_SPEED_FAST_PLUS) | I2C_MODE_MASTER;
    i2c_configure(dev_i2c, i2c_config);

    /*Prepare Data Buffer*/
    const int num_bytes = 2;
    unsigned char data_buffer[num_bytes];
    data_buffer[0] = 0x41; // 'A'
	data_buffer[1] = 0x7A; // 'z'
    uint16_t i2c_slave_address = 0x9999; // match config in slave_setup.c

	if (i2c_write(dev_i2c, data_buffer, num_bytes, i2c_slave_address)) {
		/*Turn on error LED if I2C failed to communicate*/
        gpio_pin_set(dev_led_error, PIN1, (int)true);
        k_msleep(500);
	}
}