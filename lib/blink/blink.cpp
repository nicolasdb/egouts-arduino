#include <blink.h>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


void blink (int pin, int delay=1000) {
    digitalWrite(pin, HIGH);
    vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(pin, LOW);
    vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
}
