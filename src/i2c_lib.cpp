#include "i2c_lib.h"

#include <assert.h>
#include <stdexcept>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>


int system_i2c::get_last_error()
{
    return errno;
}

system_i2c::system_i2c()
{
    h_i2c = open("/dev/i2c-1", O_RDWR);
    if (h_i2c < 0)
        throw std::runtime_error("can't open /dev/i2c-1");
}

system_i2c::system_i2c(system_i2c const&)
{
    assert(0);
}

system_i2c::~system_i2c()
{
    close(h_i2c);
}

void system_i2c::send(uint8_t address, uint8_t const* data, int len)
{
    int err;
    assert(len > 0);

    err = ioctl(h_i2c, I2C_SLAVE, address);
    if (err < 0) 
        throw std::runtime_error("ioctl error");

    err = write(h_i2c, data, len);
    if (err != len)
        throw std::runtime_error("write error");
}

void system_i2c::recv(uint8_t address, uint8_t* data, int len)
{
    int err;
    assert(len > 0);

    err = ioctl(h_i2c, I2C_SLAVE, address);
    if (err < 0) 
        throw std::runtime_error("ioctl error");

    err = read(h_i2c, data, len);
    if (err != len)
        throw std::runtime_error("write error");
}

