/**************************************************************************
Thermistors
BMEN 6000 - Signal Processing for Medical Devices 2025
 **************************************************************************/

// Fill in variable values correctly: 5 pts
// Required libraries

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// Input parameters

float R1 = 10000;
float R2 = 10000;
float R3 = 10000;
float inVoltage = 3.3;
float sampRate = 1;

// DAQ variables

int V1;
int V2;

// Temp calculation variables

float Vdiff;
float resistance;
float logR;
float SHa = 0.001129104;
float SHb = 0.000234276;
float SHc = 8.81008E-08;
float temp;

// Moving average variables

int i = 0;
const int buffsize = 10;
float tempBuff[buffsize];
float sumTemp = 0;
float avgTemp;

// Save data to SD card

const int chipSelect = 4;
File tempRecord;

// Initialize data acqusition and calculations

void setup() {

  Serial.begin(9600);
  while (!Serial);

  //Uncomment to use SD card

   SD.begin(chipSelect);
  if (!SD.begin(chipSelect)) {
    Serial.println("SD not found ");
    return;
  }
  Serial.println("SD found");

  tempRecord = SD.open("tempLog.txt", FILE_WRITE);
  if (tempRecord) {
    tempRecord.println("Voltage (1)  Voltage (2)  Voltage (diff)  Resistance  Temp (C)  Average temp (C)");
    tempRecord.close();
  }
  else {
    Serial.println("error opening tempRecord.txt");
    return;
  }

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

}

void loop() {

  // Read voltage from Wheatstone bridge

  V1 = analogRead(A2);  
  V2 = analogRead(A1);

  // Calculate difference between nodes (equivalent to voltage across bridge)
  // 3 pts

  Vdiff = (V1 - V2) / (pow(2,10) - 1) * inVoltage;

  // Calculate resistance from voltage
  // 7 pts

  resistance = (R2 * inVoltage - (R1 + R2) * Vdiff) * R3 / (R1 * inVoltage + (R1 + R2) * Vdiff);

  // Calculate temperature from resistance
  // 10 pts

  logR = log(resistance);
  temp = 1 / (SHa + SHb * logR + SHc * powf(logR, 3)) - 273.15;

  // Moving average
  // 10 pts

  sumTemp = sumTemp - tempBuff[i];
  tempBuff[i] = temp;
  sumTemp = sumTemp + tempBuff[i];
  i++;
  i %= buffsize;
  avgTemp = sumTemp / buffsize;

  // Output (Serial.print prints to the serial monitor; tempRecord.print prints to the SD card)

  Serial.print(V1);
  Serial.print("\t");
  Serial.print(V2);
  Serial.print("\t");
  Serial.print(Vdiff);
  Serial.print("\t");
  Serial.print(resistance);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\t");
  Serial.println(avgTemp);

  // Uncomment to use SD card
  
   tempRecord = SD.open("tempLog.txt", FILE_WRITE);
  
   if (tempRecord) {
     tempRecord.print(V1);
     tempRecord.print("\t");
     tempRecord.print(V2);
     tempRecord.print("\t");
     tempRecord.print(Vdiff);
     tempRecord.print("\t");
     tempRecord.print(resistance);
     tempRecord.print("\t");
     tempRecord.print(temp);
     tempRecord.print("\t");
     tempRecord.println(avgTemp);
     tempRecord.close();
   }
   else {
     Serial.println("error opening tempLog.txt");
     return;
   }
  
  // Implement sampling rate: 5 pts

  delay(1000);  
  // sampling rate is 1Hz (1 sample per second), equivalent to 1 second between samples
  // delay is the time in miliseconds between each sample
  // 1s = 1000ms
  // 1 * 1000ms/1s = 1000
}





