#include <fade.h>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

// define MCP0
Adafruit_MCP23017 mcp0;

void fadeSetup (){
  // define all buttons
  mcp0.begin(0);      // use default address 0
  //  This could probably be an array
  for (size_t i = 0; i < 9 ; i++){
    mcp0.pinMode(i, OUTPUT);
    vTaskDelay(1);
  }

}

void fadeOFF (){
  for (size_t i = 0 ; i < 9 ; i++){
    mcp0.digitalWrite(i, LOW);
    vTaskDelay(1);
    }
}

void fade (int delay=1000) {

for (size_t i = 0 ; i < 9 ; i++){
  mcp0.digitalWrite(i, HIGH);
  vTaskDelay(1);
  }
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
for (size_t i = 0 ; i < 9 ; i++){
  mcp0.digitalWrite(i, LOW);
  vTaskDelay(1);;
  }
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
}
