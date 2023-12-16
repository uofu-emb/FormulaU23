#include "BNO055_Drv.h"
#include "i2c_driver.h"

extern const struct device *i2c_dev;

bool BNO055_Begin(void) // set it to OPERATION_MODE_NDOF
{
    uint8_t reg_add = BNO055_CHIP_ID_ADDR;
    uint8_t chip_id;
    i2c_dev = I2C_Init();

    if(!i2c_dev)
    {
        return false;
    }
    // Command to read the BNO055 chip ID
    int ret = i2c_write(i2c_dev, &reg_add, 1, BNO055_ADDRESS_A);
    if (ret)
    {
        return false;
    }
    // Read the chip ID
    ret = i2c_read(i2c_dev, &chip_id, sizeof(chip_id), BNO055_ADDRESS_A);
    if (ret)
    {
        return false;
    }
    return true;
}

void BNO055_SetMode(BNO055_opmode_t mode)
{
    
}

BNO055_opmode_t getMode()
{
    return (BNO055_opmode_t)OPERATION_MODE_CONFIG;
}

void BNO055_SetAxisRemap(BNO055_axis_remap_config_t remapcode)
{

}

void BNO055_SetAxisSign(BNO055_axis_remap_sign_t remapsign)
{

}

void BNO055_GetRevInfo(BNO055_rev_info_t *info)
{

}

void BNO055_GetExtCrystalUse(bool usextal)
{

}

void BNO055_GgetSystemStatus(uint8_t *system_status, uint8_t *self_test_result,
                    uint8_t *system_error)
{

}

void BNO055_GetCalibration(uint8_t *system, uint8_t *gyro, uint8_t *accel,
                    uint8_t *mag)
{

}

Vector3 BNO055_GetVector(BNO055_vector_type_t vector_type)
{
    Vector3 xyz;
    uint8_t reg_add = vector_type;
    uint8_t buffer[6];
    memset(buffer, 0, 6);

    int16_t x, y, z;
    x = y = z = 0;
    int ret = i2c_write(i2c_dev, &reg_add, 1, BNO055_ADDRESS_A);
    if (ret)
    {}
    /* Read vector data (6 bytes) */
    // readLen((BNO055_bno055_reg_t)vector_type, buffer, 6);
    ret = i2c_read(i2c_dev, buffer, sizeof(buffer), BNO055_ADDRESS_A);
    if (ret)
    {}

    x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
    y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
    z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

    /*!
    * Convert the value to an appropriate range (section 3.6.4)
    * and assign the value to the Vector type
    */
    switch (vector_type) {
    case VECTOR_MAGNETOMETER:
        /* 1uT = 16 LSB */
        xyz.x = ((double)x) / 16.0;
        xyz.y = ((double)y) / 16.0;
        xyz.z = ((double)z) / 16.0;
        break;
    case VECTOR_GYROSCOPE:
        /* 1dps = 16 LSB */
        xyz.x = ((double)x) / 16.0;
        xyz.y = ((double)y) / 16.0;
        xyz.z = ((double)z) / 16.0;
        break;
    case VECTOR_EULER:
        /* 1 degree = 16 LSB */
        xyz.x = ((double)x) / 16.0;
        xyz.y = ((double)y) / 16.0;
        xyz.z = ((double)z) / 16.0;
        break;
    case VECTOR_ACCELEROMETER:
        /* 1m/s^2 = 100 LSB */
        xyz.x = ((double)x) / 100.0;
        xyz.y = ((double)y) / 100.0;
        xyz.z = ((double)z) / 100.0;
        break;
    case VECTOR_LINEARACCEL:
        /* 1m/s^2 = 100 LSB */
        xyz.x = ((double)x) / 100.0;
        xyz.y = ((double)y) / 100.0;
        xyz.z = ((double)z) / 100.0;
        break;
    case VECTOR_GRAVITY:
        /* 1m/s^2 = 100 LSB */
        xyz.x = ((double)x) / 100.0;
        xyz.y = ((double)y) / 100.0;
        xyz.z = ((double)z) / 100.0;
        break;
    }

  return xyz;
}