#include <fade.h>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>



void fade (int delay=1000) {

  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second

}
