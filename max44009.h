#ifndef __MAX44009_H__
#define __MAX44009_H__

#include "max44009Types.h"

/** @breif Sets up the interrupt detection window
 *
 * This function initializes the sensor interface with the I2C interface
 * to use and the address of the sensor. The address has only two valid inputs
 * based on MAX44009_ADDR_t enum but this is not checked in this function.
 *
 * @param ADDR Address of the sensor (see MAX44009_ADDR_t enum)
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_init(const uint8_t ADDR);

/** @breif Configures the max44009 light sensor
 *
 * This function sets all configuration settings at once to reduce I2C traffic.
 * the parameter should be a bitwise OR of all the settings defined in the
 * configurations enumerations.
 *
 * @param configuration bitwise OR of the configuration settings
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_configure(const uint8_t configuration);

/** @breif Enables the interrupt output of the sensor
 *
 * Please note that this functon only enables the interrupt on the sensor,
 * and for now does not set up the local detection of that interrupt.
 * Use enumeration values in LIGHT_ISR_ENABLED_t to set value.
 *
 * @param enable enable or disable ISR with enum or literal 0x00 and 0x01
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_isr(const uint8_t enable);

/** @breif Gets the most recent reading from the sensor
 *
 * The sensor works by continously sampling and storing the latest sample
 * into a register that can be read at any time. This function retreives the
 * latest sample from that register.
 *
 * @return the light value from the sensor as a float value in lux
 */
float max44009_read_float();

/** @breif Gets the most recent reading from the sensor without decoding
 *
 * The sensor works by continously sampling and storing the latest sample
 * into a register that can be read at any time. This function retreives the
 * latest sample from that register. This function returns the exponent and the mantissa in one
 * 16 bit integer to reduce computational time. bits 0-7 are the mantissa and bits 8-11 are the
 * exponent.
 *
 * @return the light value from the sensor with the LSB as the mantissa and the MSB as the exponent
 */
uint16_t max44009_read_uint16();

/** @breif Sets up the interrupt detection window
 *
 * This function sets up the light change detection
 * window. This function DOES NOT enable the interrupt,
 * which needs to be enabled or disabled seperatly.
 *
 * @param upper The upper threshold (high byte)
 * @param lower The lower threshold (high byte)
 * @param timer The amount of time (in 100ms increments) light needs to be outside the window to trigger the interrupt
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_set_window(const uint8_t upper, const uint8_t lower, const uint8_t timer);

#endif    /* __MAX44009_H__ */