#include "common.h"

#define sysclock system_clock::instance()

class system_clock
{
private:
    uint32_t freq;

    system_clock();
    system_clock(system_clock const&) { assert(0); }
    ~system_clock();

    friend void system_clock_int_handler();

public:
    static system_clock& instance();

    int64_t usec() const;
    int64_t ticks() const;
    float ticks_to_sec(int64_t t) const;

    void delay(float ms) const;
};

