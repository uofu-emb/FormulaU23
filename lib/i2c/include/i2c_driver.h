#pragma once
#include <stdbool.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>
#include <sys/printk.h>

/* I2C device information */

/* Buffer size */
#define BUFFER_SIZE 8

const struct device * I2C_Init(void);
