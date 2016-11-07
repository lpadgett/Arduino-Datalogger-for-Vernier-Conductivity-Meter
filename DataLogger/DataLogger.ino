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
  * Peristaltic Pump (PP) Pin: 3. Note: PWN is required for speed adjustment.
  */

//The ADS1015 library facilitates reading from the ADS1115.
#include <Adafruit_ADS1015.h> //V1.1
//Organize ADC variables, set base address
Adafruit_ADS1115 ads1115; //Creates instance of ads1115
Adafruit_ADS1115 ads(0x48); //Sets base address
//Create variables for displaying on LCD and saving to SD Card
float cVoltage; //16 bit variable reading from ADC's A0
String resultmV; //String containing reading from ADC in mV
String resultV; //String containing reading from ADC in V


//The LiquidCrystal library facilitates usage of the  LCD screen
#include <LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 9, 7, 6, 2); //Initialize LiquidCrystal Library


//#include <SD.h>
//Instead, use the SdFat library by greiman (https://github.com/greiman/SdFat)
#include <SdFat.h>
SdFat SD; //Added for backwards compatability. This was suggested on the Arduino forum
//by user Scottrockcafe to user trilbytim (https://forum.arduino.cc/index.php?topic=231495.0).
//Create file for saving to SD Card
String fileName = "test.txt"; //Change as needed
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
  lcd.clear();
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    file.println("testData"); //writes to SD card
    file.close();
  } else{
    LCDwrite("Error writing to", fileName);
    }
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
  delay(500);
}

void LCDwrite(String s, String s2){
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
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    file.println(resultmV); //writes to SD card
    file.close();
  } else{
    LCDwrite("Error writing to", fileName);
    delay(5000);
    }
}

void writeResult(){ //Writes result to SD card and LCD Screen
  readADS(); //Reads data from ADC
  SDwrite(); //Write data to SD card
  LCDwrite((resultV + " V"), (resultmV + " mV ")); //Space keeps unnecessary "V" from appearing
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
  LCDwrite("Writing to", fileName);
  delay(3000);
  //Writes header to file
  file.println("milliVolts");
  file.println("");
  lcd.clear();
  }




