#include "common.h"
#include <assert.h>


#define lps lps_device::instance()


class lps_device
{
private:
    lps_device();
    lps_device(lps_device const&) { assert(0); }
    ~lps_device();

public:
    static lps_device& instance()
    {
        static lps_device s;
        return s;
    }

    float pressure();
    float temperature();
};

