/*   Created on 2017-01-12 by Dominic Dotzert.
 *    
 *   Uses the Neo-6M GPS to periodically log GPS coordinates.
 *   
 *   To be contained in small enclosure worn around pet collar.
 *   
 */

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>

static const int RXPIN = 9;
static const int TXPIN = 8;

TinyGPSPlus gps;
SoftwareSerial ss(RXPIN, TXPIN);
String logFileName = "";
File logFile;

long lastUpdate = 0;

void setup() {
//  Serial.begin(115200);
  ss.begin(9600);

  // Initialize SD card
  pinMode(10,OUTPUT);
  if (!SD.begin(10)) {
//    Serial.println("SD CARD FAILED.");
    exit(1);
  }

  // Look for file containing the previous log number, then attempt
  // to read and update the file, else use log number 1
  int logNumber = 1;
  if (SD.exists("current_log.txt")) {
    //if there is a record of the logs, get the previous log #
    logFile = SD.open("current_log.txt");
    while(logFile.available()) {
      logNumber = logFile.readString().toInt();
    }
    logFile.close();
    logNumber++;
    logFile = SD.open("current_log.txt", FILE_WRITE);
    logFile.seek(0);
    logFile.print(leadingZeros3(logNumber));
    logFile.close();
  }
  else {
    //if the file doesn't exist, create one
    logFile = SD.open("current_log.txt", FILE_WRITE);
    logFile.print("001");
    logFile.close();
  }

  //create the new log file
  logFileName = "log_" + leadingZeros3(logNumber) + ".txt";
  logFile = SD.open(logFileName, FILE_WRITE);
  logFile.print("GPS LOG FILE ");
  logFile.println(leadingZeros3(logNumber));
  logFile.close();

  //enter timer here
}

String leadingZeros2(int number) {
  //returns String of integer with leading zeros
  String newNumber = "";
  if (number < 10)
    newNumber += "0";
  newNumber += String(number);
  return newNumber;
}

String leadingZeros3(int number) {
  //returns String of integer with leading zeros
  String newNumber = "";
  if (number < 100)
    newNumber += "0";
  if (number < 10)
    newNumber += "0";
  newNumber += String(number);
  return newNumber;
}

void updateSD() {
  logFile = SD.open(logFileName, FILE_WRITE);
  if (gps.date.isValid()) {
    logFile.print(gps.date.year());
    logFile.print("-");
    logFile.print(leadingZeros2(gps.date.month()));
    logFile.print("-");
    logFile.print(leadingZeros2(gps.date.day()));
  }
  else {
    logFile.print("[DATE-INVALID]");
  }
  logFile.print("T");
  if (gps.time.isValid()) {
    logFile.print(leadingZeros2(gps.time.hour()));
    logFile.print(":");
    logFile.print(leadingZeros2(gps.time.minute()));
    logFile.print(":");
    logFile.print(leadingZeros2(gps.time.second()));
  }
  logFile.print(" ");
  if (gps.location.isValid()) {
    logFile.print(gps.location.lat(), 6);
    logFile.print(",");
    logFile.println(gps.location.lng(), 6); 
  }
  else {
    logFile.println("[LOCATION-INVALID]");
  }

  logFile.close();
  lastUpdate = millis();
}

void loop() {
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (millis() - lastUpdate > 15000) {
        updateSD();
      }
    }
  }
}
