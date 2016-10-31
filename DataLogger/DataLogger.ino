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

 /*
  * Pin specifications:
  * SS for LCD: 2
  * SS for SD Card Reader/Writer ("R/W"): 10
  * MOSI Pin: 11
  * MISO Pin: 12
  * Clock Pin: 13
  * SDA and SCL are used for the ADS1115.
  * Peristaltic Pump (PP) Pin: 5. Note: PWN is required for speed adjustment.
  */

//The Wire library allows communication with I2C / TWI devices.
#include <Wire.h> //V1.0

//The ADS1015 library facilitates reading from the ADS1115.
#include <Adafruit_ADS1015.h> //V1.1

//The LiquidCrystal library, by Juan Hernandez (juanh0238) facilitates usage of the 
//LCD screen using a shift register via SPI.
#include <LiquidCrystal.h>

//The SPI library enables SPI interface bus.
#include <SPI.h>

//Set pins for MISO/MOSI/SS/PP
const int lcdSS = 2; //SS for LCD
LiquidCrystal lcd(2);
const int sdSS = 10; //SS for SD card R/W
const int MOSIpin = 11; //MOSI pin
const int MISOpin = 12; //MISO pin
const int clk = 13; //Clock pin
const int PP = 5; //Peristaltic pump pin

Adafruit_ADS1115 ads1115;
Adafruit_ADS1115 ads(0x48); //Sets base address

int16_t cVoltage; //16 bit variable reading from ADC's A0
String result; //String containing data from ADC

void setup() {
  //Set pin modes
  pinMode(lcdSS, OUTPUT);
  pinMode(sdSS, OUTPUT);
  pinMode(MOSIpin, OUTPUT);
  pinMode(MISOpin, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(PP, OUTPUT);

  //Initialize communication processes
  Serial.begin(9600); //Sets baud rate
  lcd.begin(16,2); //Initializes LCD with # of rows and columns
  ads1115.begin(); //Initializes ADC
  lcd.print(""); //Print nothing to LCD to calibrate
}

void loop() {
  writeResult();
}

void LCDwrite(String s, String s2){
  lcd.setCursor(0, 0);
  lcd.print(s);
  lcd.setCursor(0,1);
  lcd.print(s2 + " mV");
}

void readADS() {
  cVoltage = ads1115.readADC_Differential_0_1();
  result = cVoltage;
}

void writeResult(){
  readADS();
  delay(500);
  LCDwrite("Hello", result);
}



