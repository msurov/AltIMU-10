#include <assert.h>
#include <cstddef>
#include <sys/time.h>
#include <unistd.h>
#include "sysclock_lib.h"


system_clock::system_clock()
{
    freq = 1000000;
}

system_clock::~system_clock()
{
}
    
system_clock& system_clock::instance()
{
    static system_clock s;
    return s;
}

int64_t system_clock::ticks() const
{
    timeval t;
    gettimeofday(&t, NULL);

    return (int64_t)t.tv_sec * 1000000 + (int64_t)t.tv_usec;
}

int64_t system_clock::usec() const
{
    return ticks();
}

float system_clock::ticks_to_sec(int64_t t) const
{
    return (float)t / freq;
}

void system_clock::delay(float ms) const
{
    sleep(ms / 1000.0f);
}


