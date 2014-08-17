#ifndef __types__h__
#define __types__h__

#include <stdint.h>
#include <assert.h>

typedef unsigned int uint_t;

template <uint_t b> 
struct bin2dec
{
    static const uint_t val = 
        (b % 010 ? 1 : 0) |
        ((b / 010) % 010 ? 2 : 0) |
        ((b / 0100) % 010 ? 4 : 0) |
        ((b / 01000) % 010 ? 8 : 0) |
        ((b / 010000) % 010 ? 16 : 0) |
        ((b / 0100000) % 010 ? 32 : 0) |
        ((b / 01000000) % 010 ? 64 : 0) |
        ((b / 010000000) % 010 ? 128 : 0);
};

#define BIN(b) (bin2dec<0##b>::val)

#ifndef M_PI
#	define M_PI 3.141592653589793238462643383279
#endif

inline float bad_float()
{
	assert(false);
	return 0.0f;
}

#endif // __types__h__
