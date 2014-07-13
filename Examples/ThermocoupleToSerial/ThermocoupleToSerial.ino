/*******************************************************************************
* Thermocouple to serial for MAX31855 example 
*
* (c) Rocket Scream Electronics
*
* This is an example of using the MAX31855 library for Arduino to read 
* temperature from a thermocouple and send the reading to serial interfacec. 
* 
* This example code is licensed under Creative Commons Attribution-ShareAlike 
* 3.0 Unported License.
*
*******************************************************************************/

#include <SPI.h>
#include  <MAX31855.h>

//#define thermocoupleSO 12
//#define thermocoupleCLK 13
//#define DATAOUT     11 // MOSI
//#define SPICLOCK    13 // SCK

#define thermocoupleCS 3

//MAX31855  MAX31855(thermocoupleSO, thermocoupleCS, thermocoupleCLK);
MAX31855  MAX31855(thermocoupleCS);

void setup() {
  Serial.begin(57600);
  while (!Serial) ;
  Serial.println("started.");
}

void loop() {
  double temperature;
  
  MAX31855.readThermocouple(temperature);
  Serial.print("Thermocouple temperature: ");
  Serial.print(temperature);
  Serial.println(" Degree Celsius");

  MAX31855.readJunction(temperature);
  Serial.print("Junction temperature: ");
  Serial.print(temperature);
  Serial.println(" Degree Celsius");

  delay(1000);
}
