// ----------------------------------------------------------------------------
//  MAX31855 Library
//  Company: Rocket Scream Electronics
//  Website: www.rocketscream.com
// 
//  This library is licensed under Creative Commons Attribution-ShareAlike 3.0 
//  Unported License.
// 
//  Revision  Description
//  ========  ===========
//  2.00      refactor to use hardware SPI, change API
//  1.10      Added negative temperature support for both junction & thermocouple.
//  1.00      Initial public release.
// ----------------------------------------------------------------------------

#include  "MAX31855.h"

MAX31855::MAX31855(uint8_t CS, unsigned char SO, uint8_t SCK)
  : cs(CS), so(SO), sck(SCK)
{
  pinMode(so, INPUT);
  pinMode(cs, OUTPUT);
  pinMode(sck, OUTPUT);  
  digitalWrite(cs, HIGH);
  digitalWrite(sck, LOW);
}

MAX31855::MAX31855(uint8_t CS)
  : cs(CS), so(0), sck(0)
{
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);

  SPI.begin();
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(SPI_CLOCK_DIV4);
}

struct valueconfig {
  uint8_t  shift; // how much to shift right to get to the value 
  uint16_t mask;  // bits to mask for actual value
  uint8_t  sign;  // bit holding the sign
  double  factor;
};

MAX31855::status_t MAX31855::read(double &temperature, unit_t unit, bool j) {
  valueconfig S[2] = {
    { 18, 0x3FFF, 14, 0.25   }, // thermocouple
    {  4, 0x0FFF, 12, 0.0625 }  // junction
  };

  MAX31855::MAX31855_t data;
  MAX31855::status_t result;

  if (so == 0 && sck == 0 && cs > 0) {
    result = readDataSPI(data);
  }
  else {
    result = readData(data);
  }

  if (result == Ok) {
    int16_t temp = (data.value >> S[j].shift) & S[j].mask;
    if (temp & (1 << S[j].sign)) {
      temp * -temp;
    }
    
    temperature = temp * S[j].factor; // now in Celsius

    if (unit == Fahrenheit) {
      temperature = (temperature * 9.0 / 5.0) + 32; 
    }
  }

  return result;
}

MAX31855::status_t MAX31855::readThermocouple(double &temperature, unit_t unit) {
  return read(temperature, unit);
}

MAX31855::status_t MAX31855::readJunction(double &temperature, unit_t unit) {
  return read(temperature, unit, true);
}

MAX31855::status_t MAX31855::readData(MAX31855::MAX31855_t &buffer) {
  digitalWrite(cs, LOW);
  delay(1);
 
  for (uint8_t bitCount = 31; bitCount >= 0; bitCount--) {
    digitalWrite(sck, HIGH);
    if (digitalRead(so)) {
      buffer.value |= ((uint32_t)1 << bitCount);
    } 
    digitalWrite(sck, LOW);
  }
  
  digitalWrite(cs, HIGH);

  return validateResponse(buffer);
}

MAX31855::status_t MAX31855::readDataSPI(MAX31855::MAX31855_t &buffer) {
  digitalWrite(cs, LOW);
  delay(1);

  for (int8_t i = 3; i >= 0; i--) {
    buffer.bytes[i] = SPI.transfer(0x00);
  }

  digitalWrite(cs, HIGH);

  return validateResponse(buffer);
}

MAX31855::status_t MAX31855::validateResponse(MAX31855::MAX31855_t &buffer) { 
  if (buffer.Fault) {
    if (buffer.FaultOpen)        return MAX31855::FaultOpen;
    if (buffer.FaultShortGround) return MAX31855::FaultShortGround;
    if (buffer.FaultShortSupply) return MAX31855::FaultShortSupply;
  }
  return Ok;
}
