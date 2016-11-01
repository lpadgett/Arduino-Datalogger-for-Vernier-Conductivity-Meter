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

//#include <SD.h>
//Instead, use the SdFat library by greiman (https://github.com/greiman/SdFat)
#include <SdFat.h>
SdFat SD; //Added for backwards compatability. This was suggested on the Arduino forum
//by user Scottrockcafe to user trilbytim (https://forum.arduino.cc/index.php?topic=231495.0).

//Set pins for MISO/MOSI/SS/PP
const int lcdSS = 2; //SS for LCD
const int sdSS = 10; //Slave select for SD card R/W
const int MOSIpin = 11; //MOSI pin
const int MISOpin = 12; //MISO pin
const int clk = 13; //Clock pin
const int PP = 5; //Peristaltic pump pin

LiquidCrystal lcd(lcdSS); //Set LCD Slave Select to 2 in library file
Adafruit_ADS1115 ads1115; //Creates instance of ads1115
Adafruit_ADS1115 ads(0x48); //Sets base address

//Create variables for displaying on LCD and saving to SD Card
float cVoltage; //16 bit variable reading from ADC's A0
String resultmV; //String containing reading from ADC in mV
String resultV; //String containing reading from ADC in V

//Create file for saving to SD Card
File resultFile;
String fileName = "test.txt"; //Change as needed

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

  //Initialize SD card
  LCDwrite("Initializing", "SD Card");
  if(!SD.begin(sdSS)){ //If the SD card can't be initialized
  LCDwrite("Card failed ", "Fix problem"); 
  while (1); //Freeze everything until it can be
  }
  LCDwrite("SD Card", "Initialized");
  resultFile = SD.open(fileName, FILE_WRITE); //Open file that will be written to
  if(! resultFile){
  LCDwrite("Error opening", fileName + "    "); //If the file can't be opened
  while(1); //Freeze everything until it can be
  }

  //Finish initialization processes
  ads1115.begin(); //Initializes ADC
  LCDwrite("",""); //Print nothing to LCD to reset it
}

void loop() {
  writeResult();
}

void LCDwrite(String s, String s2){
  digitalWrite(sdSS, LOW); //Set SD SS to low
  digitalWrite(lcdSS, HIGH); //Set LCD SS to high
  lcd.setCursor(0, 0); //Write to LCD
  lcd.print(s);
  lcd.setCursor(0,1);
  lcd.print(s2);
}

void readADS() {
  cVoltage = ((ads1115.readADC_Differential_0_1())*0.1875);
  /*Note:
   * The ADS1115 is 16 bits. Its range is +/- 6.144v. However,
   * only 15 bits are used to communicate measurement because one
   * of the bits is used to say whether the measurement is positive
   * or negative. This leaves us with only 2^15 possible output values.
   * 6.144 volts / 32767 = 0.1875mV per unit.
   * So, we take the units of output the shift register gives us,
   * multiply that by 0.1875, and divide that by 1000 to get voltage
   * in volts.
   */
  resultmV = cVoltage;
  resultV = cVoltage/1000;
}

void SDwrite() {
  digitalWrite(lcdSS, LOW); //Sets LCD SS pin to low
  digitalWrite(sdSS, HIGH); //Sets SD SS pin to high
  resultFile.println(resultmV + ","); //writes to SD card
}

void writeResult(){ //Writes result to SD card and LCD Screen
  readADS(); //Reads data from ADC
  SDwrite(); //Write data to SD card
  LCDwrite((resultV + " V"), (resultmV + " mV ")); //Space keeps unnecessary "V" from appearing
}




