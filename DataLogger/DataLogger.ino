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

//The Wire library allows communication with I2C / TWI devices.
#include <Wire.h>

//The ADS1015 library facilitates reading from the ADS1115.
#include <Adafruit_ADS1015.h>

//The LiquidCrystal library (by Juan Hernandez, aka juanh0238) facilitates usage of the 
//LCD screen using a shift register via SPI.
#include <LiquidCrystal.h>

//The SPI library enables SPI interface bus.
#include <SPI.h>

float cVoltage = 0.0; //Conductivity meter voltage

LiquidCrystal lcd(2);

void setup() {
  Serial.begin(9600); //Sets baud rate
  lcd.begin(16,2); //Initializes LCD with # of rows and columns
//  ads.begin(); //Initializes ADC
  lcd.print("Hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  lcd.print("Hello, world!");
}
