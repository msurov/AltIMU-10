/*
 * L3GD20
 */

#include "gyro_lib.h"
#include "common.h"
#include "i2c_lib.h"
#include "sysclock_lib.h"

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>


enum {
    L3GD20_ADDRESS   = 0xD6 >> 1,
    WHO_AM_I         = 0x0F,
    L3GD20_ID        = 0xD7,
    
    L3G_CTRL_REG1    = 0x20,
    L3G_CTRL_REG2    = 0x21,
    L3G_CTRL_REG3    = 0x22,
    L3G_CTRL_REG4    = 0x23,
    L3G_CTRL_REG5    = 0x24,
    L3G_REFERENCE    = 0x25,
    L3G_OUT_TEMP     = 0x26,
    L3G_STATUS_REG   = 0x27,

    L3G_OUT_X_L      = 0x28,
    L3G_OUT_X_H      = 0x29,
    L3G_OUT_Y_L      = 0x2A,
    L3G_OUT_Y_H      = 0x2B,
    L3G_OUT_Z_L      = 0x2C,
    L3G_OUT_Z_H      = 0x2D,

    // Output data rate = 760Hz, Cut-Off (Bandwidth?)= 30Hz, all axes enabled
    CTRL_REG1_VALUE  = BIN(11001111),

    // continuos update, Data LSb, 2000 degrees per sec, I2C wire
    CTRL_REG4_VALUE  = BIN(00110000),
};


static const float coef = M_PI / 180.0 * (
    (CTRL_REG4_VALUE & BIN(110000)) == BIN(     0) ? 0.00875 : // ±250 dps
    (CTRL_REG4_VALUE & BIN(110000)) == BIN( 10000) ? 0.01750 : // ±500 dps
    (CTRL_REG4_VALUE & BIN(110000)) == BIN(100000) ? 0.070   : // ±2000 dps
    (CTRL_REG4_VALUE & BIN(110000)) == BIN(110000) ? 0.070   : // ±2000 dps
      bad_float());

static void write_to_reg(uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    i2c.send(L3GD20_ADDRESS, buf, 2);
}

std::string to_string(int x)
{
    std::stringstream s;
    s << x;
    return s.str();
}

device_gyro::device_gyro()
{
    // check device type
    uint8_t device = i2c.request(L3GD20_ADDRESS, WHO_AM_I);
    if (device != L3GD20_ID)
        throw std::runtime_error("unknown device, expected L3GD20, but received " + to_string(device));

    write_to_reg(L3G_CTRL_REG1, CTRL_REG1_VALUE);
    write_to_reg(L3G_CTRL_REG4, CTRL_REG4_VALUE);
}

device_gyro::~device_gyro()
{
}

vec3d device_gyro::angular_velocity()
{
    uint8_t buf[6];

    i2c.putc(L3GD20_ADDRESS, L3G_OUT_X_L | (1 << 7));
    i2c.recv(L3GD20_ADDRESS, buf, 6);

    int16_t const wx = (int16_t)(buf[1] << 8 | buf[0]);
    int16_t const wy = (int16_t)(buf[3] << 8 | buf[2]);
    int16_t const wz = (int16_t)(buf[5] << 8 | buf[4]);

    return vec3d(wx, wy, wz) * coef - w0;
}

bool device_gyro::calibrate()
{
    static const uint_t n = 512;
    std::vector<vec3d> meas(n);

    // measure
    for (uint_t i = 0; i < n; ++ i)
    {
        meas[i] = angular_velocity();
        sysclock.delay(1);
    }

    // mean value
    vec3d mean;

    for (uint_t i = 0; i < n; ++ i)
        mean = mean + meas[i];

    mean = mean / n;

    // standard deviation
    float dev = 0;

    for (uint_t i = 0; i < n; ++ i)
        dev += (meas[i] - mean).square();

    dev = sqrt(dev / n);

    if (dev > 0.05)
        return false;

    w0 = w0 + mean;
    return true;
}

