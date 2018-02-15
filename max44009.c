#include "max44009.h"
#include "math.h"

#define FULL_ACCURACY_CONSTANT      (0.045)
#define LOW_ACCURACY_CONSTANT       (0.720)

static struct {
    //SERCOM I2C;       /* The I2C hardware - unkown type for now */
    uint8_t address;    /* The I2C address of the sensor */

} max44009_data;

/* read a single register */
static uint8_t readReg(const uint8_t REG) {

} 

/* Function to read two registers back to back without a stop condition (for temperature) */
static void readReg2(const uint8_t REG, uint8_t* const first, uint8_t* const second) {
    
}

/* write a single register */
static void writeReg(const uint8_t REG, const uint8_t VAL) {

}

bool max44009_init(const uint8_t ADDR)
{
    max44009_data.address = ADDR;
}

bool max44009_configure(const uint8_t configuration)
{
    writeReg(LIGHT_ISR_CONFIG, configuration);
}

bool max44009_isr(const uint8_t enable)
{
    writeReg(LIGHT_ISR_ENABLE, enable);
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
}
