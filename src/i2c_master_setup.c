#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>

/*Status LED DT reference*/
#define LED0_NODE DT_ALIAS(led0)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0 DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0 DT_GPIO_FLAGS(LED0_NODE, gpios)

/*I2C DT reference*/
#define I2C_LABEL DT_PROP(DT_NODELABEL(i2c1), label)

void i2c_master_main(void){
    /*Instantiate status LED; Initialize to 1*/
    const struct device *dev_led;
    dev_led = device_get_binding(LED0);
    int ret_led = gpio_pin_configure(dev_led, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
    gpio_pin_set(dev_led, PIN0, (int)true);
    k_msleep(1000);
    
    /*Instantiate I2C peripheral in Master configuration*/
    const struct device *dev_i2c;
    dev_i2c = device_get_binding(I2C_LABEL);
    if(!device_is_ready(dev_i2c))
    {
        gpio_pin_set(dev_led, PIN0, (int)false);
        k_msleep(500);
    }
    uint32_t i2c_config = I2C_SPEED_SET(I2C_SPEED_FAST_PLUS) | I2C_MODE_MASTER;
    i2c_configure(dev_i2c, i2c_config);
}