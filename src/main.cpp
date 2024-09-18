#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include "config.h"
#include "MP3Player.h"

// Function prototypes
void handleButtonPress();
void updateLEDs();
void runScenario(int scenarioNumber);
void turnOffAllLEDs();

// Global variables
Adafruit_MCP23X17 mcp0;
Adafruit_MCP23X17 mcp1;
MP3Player mp3Player;

enum ScenarioState {
  IDLE,
  S1, S2, S3, S4, S5
} currentState = IDLE;

// MCP1 pin definitions
const int pump = 0;
const int ledCiterne = 1;
const int sterput = 2;
const int citerne = 3;
const int tImper = 4;
const int tPlant = 5;
const int egout = 6;
const int bOrage = 7;
const int ledBO2 = 8;
const int ledBO1 = 9;
const int pChamp = 10;
const int pJardin = 11;
const int collecteur = 12;
const int ruePlace = 13;
const int riviere = 14;
const int simVille = 15;

// MCP0 pin definitions
const int StripPin = 5;
const int nappe = 6;
const int egg = 7;
const int cave = 8;
const int toilette = 9;
const int sdb1 = 10;
const int arrosage = 11;
const int cuisine = 12;
const int lavelinge = 13;

const int ledButtonArray[] = {0, 1, 2, 3, 4};
const int numLeds = 5;

const int buttonPin = A3;
int buttonID = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give some time for the serial connection to establish
  
  Serial.println("Starting setup...");

  Wire.begin();
  Serial.println("I2C initialized");

  if (!mcp0.begin_I2C(MCP0_ADDRESS)) {
    Serial.println("MCP0 Error. Could not find MCP23017 chip on address 0x20");
    while (1);
  }
  Serial.println("MCP0 initialized");

  if (!mcp1.begin_I2C(MCP1_ADDRESS)) {
    Serial.println("MCP1 Error. Could not find MCP23017 chip on address 0x21");
    while (1);
  }
  Serial.println("MCP1 initialized");

  mp3Player.begin();

  pinMode(buttonPin, INPUT);

  // Configure MCP0 pins
  for (int i = 0; i < 16; i++) {
    mcp0.pinMode(i, OUTPUT);
    mcp0.digitalWrite(i, LOW);
  }

  // Configure MCP1 pins
  for (int i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp1.digitalWrite(i, LOW);
  }

  Serial.println("Setup complete");
}

void loop() {
  handleButtonPress();

  switch (currentState) {
    case IDLE:
      updateLEDs();
      break;
    case S1:
      runScenario(1);
      break;
    case S2:
      runScenario(2);
      break;
    case S3:
      runScenario(3);
      break;
    case S4:
      runScenario(4);
      break;
    case S5:
      runScenario(5);
      break;
  }

  delay(10);
}

void handleButtonPress() {
  if (currentState == IDLE) {
    int buttonValue = analogRead(buttonPin);
    delay(20);
    int buttonValue2 = analogRead(buttonPin);
    
    Serial.print("Button value: ");
    Serial.println(buttonValue);

    if (buttonValue >= 950 && buttonValue2 >= 950) {
      Serial.println("Button 1");
      buttonID = 1;
      currentState = S1;
    } else if (buttonValue >= 876 && buttonValue < 949 && buttonValue2 >= 876 && buttonValue2 < 949) {
      Serial.println("Button 2");
      buttonID = 2;
      currentState = S2;
    } else if (buttonValue >= 751 && buttonValue < 875 && buttonValue2 >= 751 && buttonValue2 < 875) {
      Serial.println("Button 3");
      buttonID = 3;
      currentState = S3;
    } else if (buttonValue >= 351 && buttonValue < 750 && buttonValue2 >= 351 && buttonValue2 < 750) {
      Serial.println("Button 4");
      buttonID = 4;
      currentState = S4;
    } else if (buttonValue >= 150 && buttonValue < 350 && buttonValue2 >= 150 && buttonValue2 < 350) {
      Serial.println("Button 5");
      buttonID = 5;
      currentState = S5;
    }

    if (currentState != IDLE) {
      turnOffAllLEDs();
    }
  }
}

void updateLEDs() {
  static unsigned long lastUpdate = 0;
  static int currentLed = 0;
  static bool direction = true;

  if (millis() - lastUpdate > 100) {
    turnOffAllLEDs();
    
    if (direction) {
      currentLed++;
      if (currentLed >= numLeds - 1) direction = false;
    } else {
      currentLed--;
      if (currentLed <= 0) direction = true;
    }

    mcp0.digitalWrite(ledButtonArray[currentLed], HIGH);
    lastUpdate = millis();
  }
}

void runScenario(int scenarioNumber) {
  Serial.print("Running Scenario ");
  Serial.println(scenarioNumber);
  
  // Turn on the corresponding LED
  mcp0.digitalWrite(buttonID - 1, HIGH);
  
  // Simulate scenario running
  delay(3000);
  
  // Turn off all LEDs
  turnOffAllLEDs();
  
  // Return to IDLE state
  currentState = IDLE;
  Serial.println("Returning to IDLE state");
}

void turnOffAllLEDs() {
  for (int i = 0; i < numLeds; i++) {
    mcp0.digitalWrite(ledButtonArray[i], LOW);
  }
}
