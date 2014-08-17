#include "matrix.h"
#include <assert.h>

#define gyro device_gyro::instance()

/*
 * singleton device_gyro
 */
class device_gyro
{
private:
    vec3d  w0;

    device_gyro();
    device_gyro(device_gyro const&) { assert(0); }
    ~device_gyro();

public:
    static device_gyro& instance() 
    {
        static device_gyro s;
        return s;
    }

    bool calibrate();
    vec3d angular_velocity();
};
