/*
 * LSM303D
 */

#include "common.h"
#include "i2c_lib.h"
#include "compass_lib.h"

#include <assert.h>
#include <cmath>
#include <stdexcept>

enum {
    I2C_MAGNETOMETER_ADDR   = BIN(11101),
    I2C_ACCELEROMETER_ADDR  = BIN(11101),

    WHO_AM_I          = 0x0F,
    LSM303D_ID        = 0x49,
    LSM303DLM_ID      = 0x3C,
      
    OUT_X_L_A         = 0x28,
    OUT_X_H_A         = 0x29,
    OUT_Y_L_A         = 0x2A,
    OUT_Y_H_A         = 0x2B,
    OUT_Z_L_A         = 0x2C,
    OUT_Z_H_A         = 0x2D,
    
    D_OUT_X_L_M       = 0x08,
    D_OUT_X_H_M       = 0x09,
    D_OUT_Y_L_M       = 0x0A,
    D_OUT_Y_H_M       = 0x0B,
    D_OUT_Z_L_M       = 0x0C,
    D_OUT_Z_H_M       = 0x0D,

    CTRL0             = 0x1F, // D
    CTRL1             = 0x20, // D
    CTRL2             = 0x21, // D
    CTRL3             = 0x22, // D
    CTRL4             = 0x23, // D
    CTRL5             = 0x24, // D
    CTRL6             = 0x25, // D
    CTRL7             = 0x26, // D
    STATUS_A          = 0x27, // D

    // acceleration data rate = 800Hz, all axes enabled
    CTRL1_VALUE = BIN(10010111),

    // Anti-alias filter = 194 Hz, (+/- 16 g full scale), I2C
    CTRL2_VALUE = BIN(01100000),

    // temperature sensor disabled, high resolution of magn sensor,
    // magn data rate = 100Hz
    CTRL5_VALUE = BIN(01110100),

    // ±4 gauss full scale
    CTRL6_VALUE = BIN(00100000),

    // low power mode off, continuous-conversion mode
    CTRL7_VALUE = BIN(0),
};

static const float gravity = 9.8f;
static const float acceleration_coef = 0.001 * gravity * (
    (CTRL2_VALUE & BIN(00111000)) == BIN(00000000) ? 0.061 : // ±2 g  
    (CTRL2_VALUE & BIN(00111000)) == BIN(00001000) ? 0.122 : // ±4 g  
    (CTRL2_VALUE & BIN(00111000)) == BIN(00010000) ? 0.183 : // ±6 g  
    (CTRL2_VALUE & BIN(00111000)) == BIN(00011000) ? 0.244 : // ±8 g  
    (CTRL2_VALUE & BIN(00111000)) == BIN(00100000) ? 0.732 : // ±16 g
      bad_float());

static const float magnetic_coef = 0.001 * (
    CTRL6_VALUE == BIN(00000000) ? 0.080 : // ±2 gauss
    CTRL6_VALUE == BIN(00100000) ? 0.160 : // ±4 gauss
    CTRL6_VALUE == BIN(01000000) ? 0.320 : // ±8 gauss
    CTRL6_VALUE == BIN(01100000) ? 0.479 : // ±12 gauss
      bad_float());

static void write_to_reg(uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    i2c.send(I2C_ACCELEROMETER_ADDR, buf, 2);
}

compass_device::compass_device()
{
    uint8_t version = i2c.request(I2C_MAGNETOMETER_ADDR, WHO_AM_I);
    if (version != LSM303D_ID)
        throw std::runtime_error("unknown device, expected LSM303D");

    write_to_reg(CTRL2, CTRL2_VALUE);
    write_to_reg(CTRL1, CTRL1_VALUE);
    write_to_reg(CTRL5, CTRL5_VALUE);
    write_to_reg(CTRL6, CTRL6_VALUE);
    write_to_reg(CTRL7, CTRL7_VALUE);
}

vec3d compass_device::acceleration()
{
    uint8_t data[6];

    i2c.putc(I2C_ACCELEROMETER_ADDR, OUT_X_L_A | (1 << 7));
    i2c.recv(I2C_ACCELEROMETER_ADDR, data, 6);

    int16_t ax = (int16_t)(data[1] << 8 | data[0]);
    int16_t ay = (int16_t)(data[3] << 8 | data[2]);
    int16_t az = (int16_t)(data[5] << 8 | data[4]);
    
    return vec3d(ax, ay, az) * acceleration_coef; // in m/s^2
}

vec3d compass_device::magnetic_field()
{
    uint8_t data[6];

    i2c.putc(I2C_MAGNETOMETER_ADDR, D_OUT_X_L_M | (1 << 7));
    i2c.recv(I2C_MAGNETOMETER_ADDR, data, 6);

    int16_t mx = (int16_t)(data[1] << 8 | data[0]);
    int16_t my = (int16_t)(data[3] << 8 | data[2]);
    int16_t mz = (int16_t)(data[5] << 8 | data[4]);
    
    return vec3d(mx, my, mz) * magnetic_coef; // in Gauss
}
