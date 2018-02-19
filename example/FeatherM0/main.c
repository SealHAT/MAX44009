#include <atmel_start.h>
#include <stdio.h>
#include "usb_start.h"
#include "max44009.h"

#define STRING_SIZE		(30)

int main(void)
{
	char  output[STRING_SIZE];
	float lightLevel = 9.0;
//	uint16_t lux;
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	max44009_init(&wire, LIGHT_ADD_GND);
		
	while (1) {
		/* Turn on LED if the DTR signal is set (serial terminal open on host) */
		gpio_set_pin_level(LED_BUILTIN, usb_dtr());
		
		/* Read the light sensor and output it to the serial USB */
		lightLevel = max44009_read_float();
//		lux = max44009_read_uint16();
		sprintf(output, "Light Level: %0lX lux\n", (uint32_t)lightLevel);
		
		if(usb_dtr()) {
			char* c = output;
			while(*c != '\0') {
				usb_put(*c);
				c++;
			}
			usb_flush();
		}
		delay_ms(100);
	}
}
