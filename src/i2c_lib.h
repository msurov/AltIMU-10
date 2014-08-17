#include "common.h"

#define i2c system_i2c::instance()


class system_i2c
{
private:
    int h_i2c;

    system_i2c();
    system_i2c(system_i2c const&);
    ~system_i2c();

public:
    void send(uint8_t address, uint8_t const* data, int len);
    void recv(uint8_t address, uint8_t* data, int len);

    static system_i2c& instance()
    {
        static system_i2c s;
        return s;
    }

    inline void putc(uint8_t address, uint8_t c)
    {
        send(address, &c, 1);
    }

    inline uint8_t getc(uint8_t address)
    {
        uint8_t res;
        recv(address, &res, 1);
        return res;
    }
    
    inline uint8_t request(uint8_t address, uint8_t code)
    {
        putc(address, code);
        return getc(address);
    }

    int get_last_error();
};




