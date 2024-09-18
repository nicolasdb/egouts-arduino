/*
 * This file is part of the MUSEE DES EGOUTS project.
 * 
 * Copyright (C) 2024 Nicolas de Barquin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include "config.h"
#include "MP3Player.h"
#include "pins.h"

#include "Scenario1Pluie.h"
#include "Scenario2EauPotable.h"
#include "Scenario3Epuration.h"
#include "Scenario4Orage.h"
#include "Scenario5RecupPluie.h"

// Function prototypes
void handleButtonPress();
void updateLEDs();
void runScenario();
void turnOffAllLEDs();

// Global variables
Adafruit_MCP23X17 mcp0;
Adafruit_MCP23X17 mcp1;
MP3Player mp3Player;

enum ScenarioState {
  IDLE,
  S1, S2, S3, S4, S5
} currentState = IDLE;

const int ledButtonArray[] = {0, 1, 2, 3, 4};
const int numLeds = 5;

const int buttonPin = A3;
int buttonID = 0;

// Scenario objects
Scenario1Pluie* scenario1;
Scenario2EauPotable* scenario2;
Scenario3Epuration* scenario3;
Scenario4Orage* scenario4;
Scenario5RecupPluie* scenario5;

// Timing variables
unsigned long lastButtonCheck = 0;
unsigned long lastLEDUpdate = 0;
unsigned long lastScenarioRun = 0;

const unsigned long buttonCheckInterval = 50;
const unsigned long ledUpdateInterval = 100;
const unsigned long scenarioRunInterval = 50;

void setup() {
  Serial.begin(115200);
  
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

  // Initialize scenario objects
  scenario1 = new Scenario1Pluie(mp3Player, mcp0, mcp1);
  scenario2 = new Scenario2EauPotable(mp3Player, mcp0, mcp1);
  scenario3 = new Scenario3Epuration(mp3Player, mcp0, mcp1);
  scenario4 = new Scenario4Orage(mp3Player, mcp0, mcp1);
  scenario5 = new Scenario5RecupPluie(mp3Player, mcp0, mcp1);

  Serial.println("Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastButtonCheck >= buttonCheckInterval) {
    handleButtonPress();
    lastButtonCheck = currentMillis;
  }

  if (currentMillis - lastLEDUpdate >= ledUpdateInterval) {
    updateLEDs();
    lastLEDUpdate = currentMillis;
  }

  if (currentMillis - lastScenarioRun >= scenarioRunInterval) {
    runScenario();
    lastScenarioRun = currentMillis;
  }
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
  static int currentLed = 0;
  static bool direction = true;

  if (currentState == IDLE) {
    turnOffAllLEDs();
    
    if (direction) {
      currentLed++;
      if (currentLed >= numLeds - 1) direction = false;
    } else {
      currentLed--;
      if (currentLed <= 0) direction = true;
    }

    mcp0.digitalWrite(ledButtonArray[currentLed], HIGH);
  }
}

void runScenario() {
  if (currentState != IDLE) {
    int scenarioNumber = static_cast<int>(currentState);
    Serial.print("Running Scenario ");
    Serial.println(scenarioNumber);
    
    // Turn on the corresponding LED
    mcp0.digitalWrite(buttonID - 1, HIGH);
    
    // Execute the appropriate scenario
    switch (currentState) {
      case S1:
        scenario1->execute();
        break;
      case S2:
        scenario2->execute();
        break;
      case S3:
        scenario3->execute();
        break;
      case S4:
        scenario4->execute();
        break;
      case S5:
        scenario5->execute();
        break;
      default:
        break;
    }
    
    // Turn off all LEDs
    turnOffAllLEDs();
    
    // Return to IDLE state
    currentState = IDLE;
    Serial.println("Returning to IDLE state");
  }
}

void turnOffAllLEDs() {
  for (int i = 0; i < numLeds; i++) {
    mcp0.digitalWrite(ledButtonArray[i], LOW);
  }
}
