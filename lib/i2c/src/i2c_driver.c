#include "i2c_driver.h"

const struct device *i2c_dev;

const struct device * I2C_Init(void)
{
    // const struct device *i2c_dev;

    i2c_dev = device_get_binding("I2C_2");

    if (!i2c_dev)
    {
        printk("I2C: Device driver not found.\n");
        return NULL;
    }
    else
    {
        return i2c_dev;
    }
}
