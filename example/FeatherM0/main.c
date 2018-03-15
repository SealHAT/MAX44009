#include <atmel_start.h>
#include <stdio.h>
#include "max44009.h"
#include "SerialPrint.h"

#define STRING_SIZE     (64)

int main(void)
{
    char  output[STRING_SIZE];
    uint16_t reading;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    max44009_init(&wire, LIGHT_ADD_GND);

    for(;;) {
        /* Turn on LED if the DTR signal is set (serial terminal open on host) */
        gpio_set_pin_level(LED_BUILTIN, usb_dtr());

        /* Read the light sensor as both a exponent/mantissa and as an integer LUX value */
        reading = max44009_read_uint16();

        /* Format as a string and output to USB Serial */
        snprintf(output, STRING_SIZE, "%04X,%d,%d,%ld\n", reading, (reading>>8), reading&0xFF, max44009_integer_lux(reading));
        if(usb_dtr()) {
            usb_write(output, strlen(output));
        }
        delay_ms(1000);
    }
}
