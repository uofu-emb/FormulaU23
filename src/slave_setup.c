#include <drivers/i2c.h>
#include <stdio.h>
#include <drivers/gpio.h>

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

const struct device *sled_dev;
const struct device *sled_dev1;
const struct device *sled_dev2;
const struct device *sled_dev3;

#define I2C_LABEL DT_PROP(DT_NODELABEL(i2c1), label)

void sled_setup()
{
    sled_dev = device_get_binding(LED0);
    sled_dev1 = device_get_binding(LED1);
    sled_dev2 = device_get_binding(LED2);
    sled_dev3 = device_get_binding(LED3);
    gpio_pin_configure(sled_dev, PIN0, GPIO_OUTPUT_INACTIVE | FLAGS0);
    gpio_pin_configure(sled_dev1, PIN1, GPIO_OUTPUT_INACTIVE | FLAGS1);
    gpio_pin_configure(sled_dev2, PIN2, GPIO_OUTPUT_INACTIVE | FLAGS2);
    gpio_pin_configure(sled_dev3, PIN3, GPIO_OUTPUT_INACTIVE | FLAGS3);

}

void sled_on(char color)
{
    if(color == 'r')
    {
        gpio_pin_set(sled_dev, PIN0, (int)true); //red
    }
    else if(color == 'o')
    {
        gpio_pin_set(sled_dev1, PIN1, (int)true); //orange
    }
    else if(color == 'g')
    {
        gpio_pin_set(sled_dev2, PIN2, (int)true); //green
    }
    else if(color == 'b')
    {
        gpio_pin_set(sled_dev3, PIN3, (int)true); //blue
    }
    
}

void sled_off(char color)
{
    if(color == 'r')
    {
        gpio_pin_set(sled_dev, PIN0, (int)false); //red
    }
    else if(color == 'o')
    {
        gpio_pin_set(sled_dev1, PIN1, (int)false); //orange
    }
    else if(color == 'g')
    {
        gpio_pin_set(sled_dev2, PIN2, (int)false); //green
    }
    else if(color == 'b')
    {
        gpio_pin_set(sled_dev3, PIN3, (int)false); //blue
    }
    
}

void slave_setup_main(){
    sled_setup();
    const struct device * i2c_device;

    i2c_device = device_get_binding(I2C_LABEL);

    // uint32_t i2c_config = I2C_SPEED_SET(I2C_SPEED_FAST_PLUS) | i2c_

    struct i2c_slave_config * slave_config;
    slave_config->address = (uint16_t)0x9999;
    // slave_config->flags = ; //what is this ???? :/
    

    int result = i2c_slave_register(i2c_device, slave_config);

    if(result == 0)
    {
        sled_on('g');
    }
    else if(result == EINVAL)
    {
        sled_on('o');
    }
    else if(result == EIO)
    {
        sled_on('r');
    }
    else if(result == ENOSYS)
    {
        sled_on('b');
    }
    else 
    {
        sled_on('g');
    }
}