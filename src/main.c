#include <zephyr.h>
#include <kernel.h>
#include "i2c_driver.h"
#include "BNO055_Drv.h"


void main(void)
{
    BNO055_Begin();
    BNO055_GetVector(VECTOR_ACCELEROMETER);

    while (1)
    {
        /* code */
    }
    
}