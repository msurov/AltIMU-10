#include "i2c_lib.h"
#include "lps_lib.h"

#include <assert.h>
#include <stdexcept>

enum {
    LPS25H_DEV_ID       = 0xBD,
    I2C_BAROMETER_ADDR  = BIN(1011101),

    PRESS_OUT_XL        = 0x28,
    TEMP_OUT_L          = 0x2B,
    WHO_AM_I            = 0x0F,       
    CTRL_REG1           = 0x20,
    CTRL1_VALUE         = BIN(10110000), // active mode, pressure freq = 12.5Hz, temp freq = 12.5Hz, I2C wire
};

static const float pressure_coef     = 100.0 / 4096.0; // in Pa
static const float temperature_coef1 = 1.0 / 480.0;
static const float temperature_coef2 = 42.5;

lps_device::lps_device()
{
    // check the sensor model
    uint8_t version = i2c.request(I2C_BAROMETER_ADDR, WHO_AM_I);
    if (version != LPS25H_DEV_ID)
      throw std::runtime_error("unknown device, expected LPS25H");

    // set sensor mode
    uint8_t buf[] = {CTRL_REG1, CTRL1_VALUE};
    i2c.send(I2C_BAROMETER_ADDR, buf, 2);
}

lps_device::~lps_device()
{
}

/*
 * returns pressure in Pa
 */
float lps_device::pressure() 
{
    uint8_t buf[4];

    buf[0] = PRESS_OUT_XL | (1 << 7);
    i2c.send(I2C_BAROMETER_ADDR, buf, 1);
    i2c.recv(I2C_BAROMETER_ADDR, buf, 3);

    int32_t pressure_raw = (int32_t)buf[2] << 16 | (uint16_t)buf[1] << 8 | buf[0];
    return pressure_raw * pressure_coef;
}

float lps_device::temperature()
{
    uint8_t buf[4];
  
    buf[0] = TEMP_OUT_L | (1 << 7);
    i2c.send(I2C_BAROMETER_ADDR, buf, 1);
    i2c.recv(I2C_BAROMETER_ADDR, buf, 2);

    int16_t temperature_raw = (int16_t)buf[1] << 8 | buf[0];
    return temperature_coef2 + temperature_coef1 * temperature_raw; // in Celsius
}







