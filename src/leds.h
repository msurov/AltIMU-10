#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>


#define leds system_leds::instance()

class system_leds
{
private:
    system_leds()
    {
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4);
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_5);
    }

    system_leds(system_leds const& ) { assert(0); }

public:
    static system_leds& instance()
    {
        static system_leds s;
        return s;
    }

    inline void yellow(bool on)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, on ? ~GPIO_PIN_5 : GPIO_PIN_5);
    }

    inline void green(bool on)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, on ? ~GPIO_PIN_4 : GPIO_PIN_4);
    }
};
