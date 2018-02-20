#include <atmel_start.h>
#include <stdio.h>
#include "usb_start.h"
#include "max44009.h"

#define STRING_SIZE     (64)

int main(void)
{
    char  output[STRING_SIZE];
    uint16_t reading;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    max44009_init(&wire, LIGHT_ADD_GND);

    while (1) {
        /* Turn on LED if the DTR signal is set (serial terminal open on host) */
        gpio_set_pin_level(LED_BUILTIN, usb_dtr());

        /* Read the light sensor as both a exponent/mantissa and as an integer LUX value */
        reading = max44009_read_uint16();

        /* Format as a string and output to USB Serial */
        sprintf(output, "%04X (exp=%2d  mant=%3d) -> %ld lux\n", reading, (reading>>8), reading&0xFF, max44009_integer_lux(reading));
        if(usb_dtr()) {
            char* c = output;
            while(*c != '\0') {
                usb_put(*c);
                c++;
            }
            usb_flush();
        }
        delay_ms(1000);
    }
}
