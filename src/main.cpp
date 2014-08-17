#include <iostream>
#include <exception>

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "i2c_lib.h"
#include "lps_lib.h"
#include "compass_lib.h"
#include "gyro_lib.h"
#include "sysclock_lib.h"

#include "quaternion.h"
#include "udp_sender.h"


int main()
{
    // init udp
    std::cout << "init UDP" << std::endl;
    udp_sender udp("192.168.137.1", 18111);

    // init & calibrate gyro
    std::cout << "initializing gyro" << std::endl;

    while (true)
    {
        if (gyro.calibrate())
        {
            std::cout << "calibrated" << std::endl;
            break;
        }

        std::cout << "couldn't calibrate" << std::endl;
    }

    // evaluate attitude
    quat q = quat::rotation(0.0, vec3d(1, 0, 0));

    // all the variables are in usec
    int64_t const   desc = 50 * 1000;
    int64_t         t_up, t1, t2;

    t_up = 
    t1 = 
    t2 = sysclock.usec();

    while (true)
    {
        vec3d   w;

        t_up = t_up + desc;

        do
        {
            w = gyro.angular_velocity();

            t1 = t2;
            t2 = sysclock.usec();

            q = q + 0.5 * q * w * (t2 - t1) * 0.000001; // usec to sec
            q.normalize(); // to fix integration error
        }
        while (t_up > t2);

        udp.send(q);
    }
}











