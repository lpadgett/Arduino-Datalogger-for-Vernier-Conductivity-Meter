/* DATALOGGER FOR SENIOR CAPSTONE AND SCIENCE FAIR
 * By Luke Padgett, Plano Academy High School, 2016-17.
 * 
 * This datalogger reads voltages automatically from a Vernier 
 * conductivity probe, transforms those analog voltage readings
 * into digital signals via an ADS1115 ADC, sends those voltages to an 
 * Arduino Uno, which transforms those voltage values into ppm (parts 
 * per million) values, and finally sends those values to a datalogging
 * module to be recorded in .csv files, depending on the test cycle ID.
 * This datalogger also controls a peristaltic pump, though that
 * functionality may be removed if one desires after downloading these files.
 * 
 * All code in these files is written by Luke Padgett unless specified otherwise
 * in the header(s) of a file, such as the Adafruit_ADS1015.h file.
 */

//The Wire.h library allows communication with I2C / TWI devices.
#include <Wire.h>

#include <Adafruit_ADS1015.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
