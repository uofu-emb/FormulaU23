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

int i2c_master_main(void){
    const struct device *dev_led;
    dev_led = device_get_binding(LED0);
    int ret_led = gpio_pin_configure(dev_led, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
    gpio_pin_set(dev_led, PIN0, (int)true);
    
    const struct device *dev_i2c;
    dev_i2c = device_get_binding(I2C_LABEL);
}