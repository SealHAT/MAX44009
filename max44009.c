#include "max44009.h"
#include "math.h"

#define FULL_ACCURACY_CONSTANT      (0.045)
#define LOW_ACCURACY_CONSTANT       (0.720)

static struct {
    //SERCOM I2C;       /* The I2C hardware - unkown type for now */
    uint8_t address;    /* The I2C address of the sensor */
	struct io_descriptor *wire_io;
} max44009_data;

/* read a single register */
static uint8_t readReg(const uint8_t REG) {
	uint8_t retval;
	io_write(max44009_data.wire_io, &REG, 1);
	io_read(max44009_data.wire_io, &retval, 1);
	return retval;
} 

/* Function to read two registers back to back without a stop condition (for temperature) */
static void readReg2(const uint8_t REG, uint8_t* const first, uint8_t* const second) {
    uint8_t retvals[2];	
	io_write(max44009_data.wire_io, &REG, 1);
    io_read(max44009_data.wire_io, retvals, 2);
	*first  = retvals[0];
	*second = retvals[1];
}

/* write a single register */
static void writeReg(const uint8_t REG, const uint8_t VAL) {
	uint8_t sendvals[2];
	sendvals[0] = REG;
	sendvals[1] = VAL;
	io_write(max44009_data.wire_io, sendvals, 2);
}

bool max44009_init(struct i2c_m_sync_desc *const WIRE_I2C, const uint8_t ADDR)
{
	max44009_data.address = ADDR;
	i2c_m_sync_get_io_descriptor(WIRE_I2C, &max44009_data.wire_io);
    i2c_m_sync_enable(WIRE_I2C);
    i2c_m_sync_set_slaveaddr(WIRE_I2C, max44009_data.address, I2C_M_SEVEN);
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

float max44009_read_float()
{
    uint8_t luxHigh;
    uint8_t luxLow;
    uint8_t exponent;
    uint8_t mantissa;

    luxHigh = readReg(LIGHT_LUX_MSB);
    luxLow = readReg(LIGHT_LUX_LSB);
    exponent = (luxHigh & 0xF0) >> 4;
    mantissa = ((luxHigh & 0x0F) << 4) | (luxLow & 0x0F);

    return pow(2, exponent) * mantissa * FULL_ACCURACY_CONSTANT;
}

uint16_t max44009_read_uint16()
{
    uint16_t luxVal = 0x0000;

    luxVal |= readReg(LIGHT_LUX_MSB) << 4;
    luxVal |= (readReg(LIGHT_LUX_LSB) & 0x0F);

    return luxVal;
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
