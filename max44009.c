#include "max44009.h"
#include "math.h"

#define FULL_ACCURACY_CONSTANT      (0.045)
#define LOW_ACCURACY_CONSTANT       (0.720)

static struct i2c_m_sync_desc max44009_sync;

/* read a single register */
static uint8_t readReg(const uint8_t REG)
{
    uint8_t retval;
    i2c_m_sync_cmd_read(&max44009_sync, REG, &retval, 1);
    return retval;
}

/* write a single register */
static void writeReg(const uint8_t REG, const uint8_t VAL)
{
    uint8_t sendval;
    i2c_m_sync_cmd_write(&max44009_sync, REG, &sendval, 1);
}

bool max44009_init(struct i2c_m_sync_desc* const WIRE_I2C, const uint8_t ADDR)
{
    max44009_sync =  *WIRE_I2C;
    i2c_m_sync_enable(&max44009_sync);
    i2c_m_sync_set_slaveaddr(&max44009_sync, ADDR, I2C_M_SEVEN);
    return true;
}

bool max44009_configure(const uint8_t configuration)
{
    writeReg(LIGHT_ISR_CONFIG, configuration);
    return true;
}

bool max44009_isr(const uint8_t enable)
{
    writeReg(LIGHT_ISR_ENABLE, enable);
    return true;
}

uint16_t max44009_read_uint16()
{
    uint16_t luxVal = 0x0000;

    luxVal |= readReg(LIGHT_LUX_MSB) << 4;
    luxVal |= (readReg(LIGHT_LUX_LSB) & 0x0F);

    return luxVal;
}

uint32_t max44009_read_integer_lux()
{
    uint16_t value = max44009_read_uint16();
    uint8_t exponent;
    uint8_t mantissa;

    exponent = value >> 8;
    mantissa = value & 0xFF;

    return pow(2, exponent) * mantissa * FULL_ACCURACY_CONSTANT;
}

uint32_t max44009_integer_lux(const uint16_t reading)
{
    uint8_t exponent;
    uint8_t mantissa;

    exponent = reading >> 8;
    mantissa = reading & 0xFF;

    return pow(2, exponent) * mantissa * FULL_ACCURACY_CONSTANT;
}

float max44009_read_float()
{
    uint16_t value = max44009_read_uint16();
    uint8_t exponent;
    uint8_t mantissa;

    exponent = value >> 8;
    mantissa = value & 0xFF;

    return pow(2, exponent) * (float)mantissa * FULL_ACCURACY_CONSTANT;
}

bool max44009_set_window(const uint8_t upper, const uint8_t lower, const uint8_t timer)
{
    // TODO
    // upper and lower will need to be calculated since device wants mantissa and exp
    writeReg(LIGHT_UPPER_LIMIT, upper);
    writeReg(LIGHT_LOWER_LIMIT, lower);
    writeReg(LIGHT_TIMER, timer);
    return true;
}
