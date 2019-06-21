#include <Arduino.h>
#include <mp3.h>
// #include <SoftwareSerial.h>
// #include <DFRobotDFPlayerMini.h>

// int StripPin = 9;    // LED strip (via MOSFET) connected to pin 9
// int rxPin = 10;    // DFplayer RX to Arduino pin 10
// int txPin = 11;    // DFplayer TX toArduinopin 11
// int busyPin = 12;  // DFplayer BUSY connected to pin 12
//
// SoftwareSerial mySoftwareSerial(rxPin, txPin);
// DFRobotDFPlayerMini myDFPlayer;

void mp3 (char* filename) {
    Serial.print("Son : ");
    Serial.println(filename);
}
