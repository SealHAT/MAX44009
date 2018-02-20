#ifndef __MAX44009_H__
#define __MAX44009_H__

#include <atmel_start.h>    /* where the IO functions live */
#include <stdint.h>
#include <stdbool.h>
#include "max44009Types.h"

/** @brief Sets up the interrupt detection window
 *
 * This function initializes the sensor interface with the I2C interface
 * to use and the address of the sensor. The address has only two valid inputs
 * based on MAX44009_ADDR_t enum but this is not checked in this function.
 *
 * @param i2c
 * @param ADDR Address of the sensor (see MAX44009_ADDR_t enum)
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_init(struct i2c_m_sync_desc* const WIRE_I2C, const uint8_t ADDR);

/** @brief Configures the max44009 light sensor
 *
 * This function sets all configuration settings at once to reduce I2C traffic.
 * the parameter should be a bitwise OR of all the settings defined in the
 * configurations enumerations.
 *
 * @param configuration bitwise OR of the configuration settings
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_configure(const uint8_t configuration);

/** @brief Enables the interrupt output of the sensor
 *
 * Please note that this function only enables the interrupt on the sensor,
 * and for now does not set up the local detection of that interrupt.
 * Use enumeration values in LIGHT_ISR_ENABLED_t to set value.
 *
 * @param enable enable or disable ISR with enum or literal 0x00 and 0x01
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_isr(const uint8_t enable);

/** @brief Gets the most recent reading from the sensor without decoding
 *
 * The sensor works by continuously sampling and storing the latest sample
 * into a register that can be read at any time. This function retrieves the
 * latest sample from that register. This function returns the exponent and the mantissa in one
 * 16 bit integer to reduce computational time. bits 0-7 are the mantissa and bits 8-11 are the
 * exponent. The high 4 bits will be set to 0.
 *
 * @return the light value from the sensor with the LSB as the mantissa and the MSB as the exponent
 */
uint16_t max44009_read_uint16();

/** @brief Gets the most recent reading from the sensor without decoding
 *
 * The sensor works by continuously sampling and storing the latest sample
 * into a register that can be read at any time. This function retrieves the
 * latest sample from that register. This function returns the lux value as an
 * unsigned integer, discarding the decimal portion of the value. Lux can range
 * from 0 to 188,006.
 *
 * @return the light value from the sensor with the LSB as the mantissa and the MSB as the exponent
 */
uint32_t max44009_read_integer_lux();

/** @brief Gets the most recent reading from the sensor without decoding
 *
 * This function returns the lux value as an unsigned integer, discarding
 * the decimal portion of the value. Lux can range from 0 to 188,006.
 *
 * @return the light value from the sensor with the LSB as the mantissa and the MSB as the exponent
 */
uint32_t max44009_integer_lux(const uint16_t reading);

/** @brief Gets the most recent reading from the sensor
 *
 * The sensor works by continuously sampling and storing the latest sample
 * into a register that can be read at any time. This function retrieves the
 * latest sample from that register. This function does not work righ now
 * on the SAMD21G chip due to float issues.
 *
 * @return the light value from the sensor as a float value in lux
 */
float max44009_read_float();

/** @brief Sets up the interrupt detection window
 *
 * This function sets up the light change detection
 * window. This function DOES NOT enable the interrupt,
 * which needs to be enabled or disabled separately.
 *
 * @param upper The upper threshold (high byte)
 * @param lower The lower threshold (high byte)
 * @param timer The amount of time (in 100ms increments) light needs to be outside the window to trigger the interrupt
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool max44009_set_window(const uint8_t upper, const uint8_t lower, const uint8_t timer);

#endif    /* __MAX44009_H__ */