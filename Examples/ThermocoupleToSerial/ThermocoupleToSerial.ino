/*******************************************************************************
ino
* Thermocouple to serial for MAX31855 example 
* Version: 1.00
* Date: 26-12-2011
* Company: Rocket Scream Electronics
* Website: www.rocketscream.com
*
* This is an example of using the MAX31855 library for Arduino to read 
* temperature from a thermocouple and send the reading to serial interfacec. 
* Please check our wiki (www.rocketscream.com/wiki) for more information on 
* using this piece of library.
*
* This example code is licensed under Creative Commons Attribution-ShareAlike 
* 3.0 Unported License.
*
* Revision  Description
* ========  ===========
* 1.00      Initial public release.
*
*******************************************************************************/

#include <SPI.h>
#include  <MAX31855.h>


#define thermocoupleSO 12
#define thermocoupleCS 10
#define thermocoupleCLK 13

//MAX31855  MAX31855(thermocoupleSO, thermocoupleCS, thermocoupleCLK);
MAX31855  MAX31855(thermocoupleCS);

void  setup()
{
  Serial.begin(57600);
}

void  loop()
{
  double temperature;
  
  //temperature = MAX31855.readThermocouple(MAX31855::Celsius);
  MAX31855.readThermocouple(temperature);
  Serial.print("Thermocouple temperature: ");
  Serial.print(temperature);
  Serial.println(" Degree Celsius");
  
  //temperature = MAX31855.readJunction(MAX31855::Celsius);
  MAX31855.readJunction(temperature);
  Serial.print("Junction temperature: ");
  Serial.print(temperature);
  Serial.println(" Degree Celsius");
  
  delay(1000);
}
