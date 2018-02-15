#include <Wire.h>

#define LIGHT_ADDR (0x4A)     // MAX44009 address when A0 is at ground
#define TEMP_ADDR  (0x40)     // si7051 temperature sensor address

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // SETUP THE LIGHT SENSOR
  Wire.beginTransmission(LIGHT_ADDR);
  // Select configuration register
  Wire.write(0x02);
  // default mode
  Wire.write(0x00);
  Wire.endTransmission();
  
  delay(100);

  // SETUP THE TEMPERATURE SENSOR
  Wire.beginTransmission(TEMP_ADDR);
  // select control register
  Wire.write(0xE6);
  // set to 14 bit resolution
  Wire.write(0x0);
  Wire.endTransmission();
}

float getLight() {
  unsigned int data[2];

  // Start I2C Transmission
  Wire.beginTransmission(LIGHT_ADDR);
  // Select data register
  Wire.write(0x03);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(LIGHT_ADDR, 2u);

  // Read 2 bytes of data
  // luminance msb, luminance lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to lux
  int exponent = (data[0] & 0xF0) >> 4;
  int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);

  // return luminance
  return pow(2, exponent) * mantissa * 0.045;
}

float getTemp() {
  Wire.beginTransmission(TEMP_ADDR);
  Wire.write(0xF3);
  Wire.write(TEMP_ADDR);
  Wire.write(TEMP_ADDR);
  Wire.write(TEMP_ADDR);
  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(TEMP_ADDR, 2u);

  byte msb = Wire.read();
  byte lsb = Wire.read();

  uint16_t val = msb << 8 | lsb;

  return ((175.72*val) / 65536) - 46.85;
}

void loop()
{
  digitalWrite(LED_BUILTIN, LOW);
  
  // get and output the light level
  Serial.print("Ambient Light luminance :");
  Serial.print(getLight());
  Serial.print(" lux\t");

  // get and output the temperature
  Serial.print("Ambient Temperature :");
  Serial.print(getTemp());
  Serial.println(" degrees C");
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}
