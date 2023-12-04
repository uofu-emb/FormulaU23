#include <drivers/i2c.h>

#define I2C_LABEL DT_PROP(DT_NODELABEL(i2c1), label)

void slave_setup_main(){
const struct device * i2c_device;

i2c_device = device_get_binding(I2C_LABEL);

// uint32_t i2c_config = I2C_SPEED_SET(I2C_SPEED_FAST_PLUS) | i2c_

struct i2c_slave_config * slave_config;
slave_config->address = (uint16_t)0x9999;
// slave_config->flags = ; //what is this ???? :/

i2c_slave_register(i2c_device, slave_config);



}