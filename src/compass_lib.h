#include "matrix.h"
#include <assert.h>

#define compass         compass_device::instance()


class compass_device
{
private:
    compass_device();
    compass_device(compass_device const&) { assert(0); }

public:
    static compass_device& instance()
    {
        static compass_device s;
        return s;
    }

    vec3d acceleration();
    vec3d magnetic_field();
};
