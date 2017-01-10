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
  * SS for SD Card Reader/Writer ("R/W"): 10
  * MOSI Pin: 11
  * MISO Pin: 12
  * Clock Pin: 13
  * SDA and SCL are used for the ADS1115.
  * Peristaltic Pump (PP) Pin: 3. Note: Adjust voltage on supply 
  * to adjust the speed of the motor. It's cheaper than creating 
  * a voltage divider with digital potentiometers.
  */

//The ADS1015 library facilitates reading from the ADS1115.
#include <Adafruit_ADS1015.h> //V1.1
//Organize ADC variables, set base address
Adafruit_ADS1115 ads1115; //Creates instance of ads1115
Adafruit_ADS1115 ads(0x48); //Sets base address
//Create variables for displaying on LCD and saving to SD Card
const String voltage = "mV"; //Change as needed
const String conductivityText = "uS"; //Change as needed
const String timeText = "ms"; //Change as needed
double cVoltage; //Variable reading from ADC's A0
String resultmV; //String containing reading from ADC in mV
String resultS; //String variable containing result in microsiemens.
double resultSd; //Double variable for deciding if measurement is not error
const double intercept = 0; //Intercept for conductivity meter, provided by Vernier*
const double slope = 0.357142857; //Slope for conductivity meter, provided by Vernier*
//*THE INTERCEPT AND SLOPE ARE NOT YET CORRECT, STILL WAITING ON A RESPONSE
//FROM VERNIER SUPPORT. 
/*Note: Voltage in mV for 1000uS/cm conductivity is mV.
//CALIBRATE USING THAT VOLTAGE. Ensure the Vernier conductivity meter's range is
*set to 0 to 20,000 uS/cm (0 to 10,000 mg/L TDS).
*/

/*IMPORTANT NOTE REGARDING THE PUMP:
* 40mL/minute speed is at 9v.
* 20mL/minute speed is at 6v.
* 10mL/minute speed is at 4v.
*/

//The LiquidCrystal library facilitates usage of the  LCD screen
#include <LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 9, 7, 6, 2); //Initialize LiquidCrystal Library


//#include <SD.h>
//Instead, use the SdFat library by greiman (https://github.com/greiman/SdFat)
#include <SdFat.h>
SdFat SD; //Added for backwards compatability. This was suggested on the Arduino forum
//by user Scottrockcafe to user trilbytim (https://forum.arduino.cc/index.php?topic=231495.0).
//Create file for saving to SD Card
String fileName = "calibration.txt"; //Change as needed
#define SD_CS_PIN SS //Defines CS pin as SS for SD card
SdFile file;


//Set pins for MISO/MOSI/SS/PP
const int sdSS = 10; //SS for sd card logger was already defined, but
//pin 10 (the hardware SS pin) must be set to output or the SdFat library
//won't work
const int MOSIpin = 11; //MOSI pin
const int MISOpin = 12; //MISO pin
const int clk = 13; //Clock pin
const int PP = 3; //Peristaltic pump pin
boolean pumpOn = false; //boolean for turning pump on and off

//Time-keeping variable
//unsigned long time;

//Set values for time it takes to circulate the 100mL 
//bulk solution 5x through apparatus at pump speeds
const int mlSpeed40 = 150000;
const int mlSpeed20 = 300000;
const int mlSpeed10 = 600000;

//Record how many times water has been circulated through apparatus
int cycle = 0;

void setup() {
  //Set pin modes
  pinMode(MOSIpin, OUTPUT);
  pinMode(MISOpin, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(PP, OUTPUT);

  lcd.begin(16,2); //Initializes LCD with # of rows and columns

  //Initialize ADC
  ads1115.begin(); //Initializes ADC
  
  titleAndInitialize();
  LCDwrite("Beginning", "Logging");
  delay(3000);
  lcd.clear();
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    file.println(conductivityText + "," + voltage + "," + timeText); //writes to SD card
    file.close();
  } else{
    LCDwrite("Error writing to", fileName);
    while(1);
    }
    pumpOn = true;
    digitalWrite(PP, HIGH); //Sets speed of peristaltic pump to a constant value
    delay(100);
}

/*
 * If void loop() isn't running...
 * Does the ADC have power?
 * Does the SD card module have power?
 * Does everything that should have power have power?
 * Is there enough power?
 * Are there any shorts?
 * Are all necessary wires connected?
 */
void loop() {
  writeResult();
  int ms = millis();
  if (ms / mlSpeed40 <= 1.01) { 
  int cycle = cycle + 1; // Add 1 to cycle number
    }
  if (cycle >= 5){
    LCDwrite("Data Collection", "Completed");
    while(1);
    }
    delay(300);
}

void LCDwrite(String s, String s2){ //Fix problem: 100ms delay makes screen almost unreadable
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
   resultSd = intercept + cVoltage*slope; //Fix later so this actually shows microsiemens
   resultS = resultSd;
   resultmV = cVoltage;
}

void SDwrite() {
  File file = SD.open(fileName, FILE_WRITE);
  if (!file){
    LCDwrite("Error writing to", fileName);
    while(1);
    }
    else {
      if (resultSd > 0) {
    file.println(resultS + "," + resultmV + "," + millis()); //writes to SD card
    file.close();
  } else {
    delay(0);
    }
    }
}

void writeResult(){ //Writes result to SD card and LCD Screen
  readADS(); //Reads data from ADC
  SDwrite(); //Write data to SD card
  LCDwrite((resultS + " uS"), (resultmV + " mV")); //Space keeps unnecessary "V" from appearing
}

void titleAndInitialize(){
  LCDwrite("Data Acquisition", "Instrument V2.0");
  delay(3000);
  lcd.clear();
  LCDwrite("By Luke Padgett", "2016-2017, PAHS");
  delay(3000);
  lcd.clear();
  LCDwrite("Detecting SD.  ", "");
  delay(500);
  LCDwrite("Detecting SD..", "");
  delay(500);
  LCDwrite("Detecting SD...", "");
  delay(500);
  //Begin SD Card module
  if (!SD.begin(SD_CS_PIN)) {
    lcd.clear();
    LCDwrite("Failed to Detect", "SD. Fix issue.");
    while(1);
  }
  if (SD.begin(SD_CS_PIN)) {
    lcd.clear();
    LCDwrite("SD Card", "Detected");
    delay(3000);
    lcd.clear();
  }
  char filename[fileName.length()+1];
  fileName.toCharArray(filename, sizeof(filename));
  if (!SD.open(fileName, FILE_WRITE)) {
    LCDwrite("Error opening", fileName);
    while(1);
  }
  file.close();
  LCDwrite("Writing to", fileName);
  delay(3000);
  lcd.clear();
  }
