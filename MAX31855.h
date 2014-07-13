// ----------------------------------------------------------------------------
//  MAX31855 Library
//  (c) 2014 karl@pitrich.com
//  (c) Rocket Scream Electronics
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

#ifndef	__have_MAX31855_H__
#define __have_MAX31855_H__

#include <Arduino.h>
#include <SPI.h>

// ----------------------------------------------------------------------------

class MAX31855 {
public:
  typedef enum status_e {
  	Unknown          = (1<<0),
  	Ok               = (1<<1),
  	FaultOpen		     = (1<<2),
  	FaultShortGround = (1<<3),
  	FaultShortSupply = (1<<4)
  } status_t;

  typedef enum unit_s {
  	Celsius    = (1<<0),
    Fahrenheit = (1<<1)
  } unit_t;

  typedef union {
    uint32_t value;
    uint8_t bytes[4];
    struct {
      uint8_t b31:1;
      uint8_t b30:1;
      uint8_t b29:1;
      uint8_t b28:1;
      uint8_t b27:1;
      uint8_t b26:1;
      uint8_t b25:1;
      uint8_t b24:1;
      uint8_t b23:1;
      uint8_t b22:1;
      uint8_t b21:1;
      uint8_t b20:1;
      uint8_t b19:1;
      uint8_t b18:1;
      uint8_t Reserved2:1;
      uint8_t Fault:1;
      uint8_t b15:1;
      uint8_t b14:1;
      uint8_t b13:1;
      uint8_t b12:1;
      uint8_t b11:1;
      uint8_t b10:1;
      uint8_t b9:1;
      uint8_t b8:1;
      uint8_t b7:1;
      uint8_t b6:1;
      uint8_t b5:1;
      uint8_t b4:1;
      uint8_t Reserved1:1;
      uint8_t FaultShortSupply:1;
      uint8_t FaultShortGround:1;
      uint8_t FaultOpen:1;
    };
  } __attribute__((packed)) MAX31855_t;

public:
  MAX31855(uint8_t CS);
  MAX31855(uint8_t CS, uint8_t SO, uint8_t SCK);

  status_t readThermocouple(double &value, unit_t unit = Celsius);
  status_t readJunction(double &value, unit_t unit = Celsius);

private:
  uint8_t so, cs, sck;  
  status_t readData(MAX31855_t &buffer);
  status_t readDataSPI(MAX31855_t &buffer);
  status_t validateResponse(MAX31855_t &buffer);

  status_t read(double &temperature, unit_t unit, bool junction = false);
};

#endif // __have_MAX31855_H__
