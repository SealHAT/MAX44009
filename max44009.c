#include "max44009.h"
#include "math.h"

#define FULL_ACCURACY_CONSTANT      (0.045)
#define LOW_ACCURACY_CONSTANT       (0.720)

static struct i2c_m_async_desc max44009_async;
static volatile bool tx_done = false;
static volatile bool rx_done = false;

static void tx_cb(struct i2c_m_async_desc *const i2c)
{
	tx_done = true;
}

static void rx_cb(struct i2c_m_async_desc *const i2c)
{
	rx_done = true;
}

/* read a single register */
static uint8_t readReg(const uint8_t REG)
{
    uint8_t retval;
    i2c_m_async_cmd_read(&max44009_async, REG, &retval);
	
	while(!rx_done) {}
	rx_done = false;
	
    return retval;
}

/* write a single register */
static void writeReg(const uint8_t REG, const uint8_t VAL)
{
    i2c_m_async_cmd_write(&max44009_async, REG, VAL);
	
	while(!tx_done) {}
	tx_done = false;
}

bool max44009_init(struct i2c_m_async_desc* const WIRE_I2C, const uint8_t ADDR)
{
    max44009_async =  *WIRE_I2C;
    i2c_m_async_enable(&max44009_async);
	i2c_m_async_register_callback(&max44009_async, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)tx_cb);
	i2c_m_async_register_callback(&max44009_async, I2C_M_ASYNC_RX_COMPLETE, (FUNC_PTR)rx_cb);
    i2c_m_async_set_slaveaddr(&max44009_async, ADDR, I2C_M_SEVEN);
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
