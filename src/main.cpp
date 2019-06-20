#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <blink.h>

// define two tasks for Blink & AnalogRead
void TaskDemo( void *pvParameters );
TaskHandle_t xDemoHandle = NULL;
void TaskButtons( void *pvParameters );
void TaskActions( void *pvParameters );

enum {
  IDLE,
  PLUIE,
  ORAGE,
  TOILETTE
} etats;

// the setup function runs once when you press reset or power the board
void setup() {

  int startPin = 3;
  for (size_t i = 0; i < 3; i++)
  {
    pinMode(i + startPin, OUTPUT);
    delay(10);
  }

  pinMode(7, OUTPUT);

  pinMode(A0, INPUT);
  
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // Now set up two tasks to run independently.
  xTaskCreate(
      TaskDemo
      ,  (const portCHAR *)"Demo"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  &xDemoHandle );

  xTaskCreate(
    TaskButtons
    ,  (const portCHAR *)"Buttons"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskActions
    ,  (const portCHAR *)"Actions"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  //
}

void suspendDemo () {
  vTaskSuspend(xDemoHandle);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDemo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) {
    // Si on est en IDLE on joue la démo
    blink(3);
    blink(4);
    blink(5);
  }
}

void TaskButtons(void *pvParameters)
{
  (void) pvParameters;

  for (;;) {

    if (etats == IDLE) {
      int buttonValue = analogRead(A0);

      if (buttonValue > 300 && buttonValue < 500) {
        Serial.println("Button 1");
        etats = PLUIE;
      } else if (buttonValue > 501 && buttonValue < 900) {
        Serial.println("Button 2");
        etats = ORAGE;
      } else if (buttonValue > 901) {
        Serial.println("Button 3");
        etats = TOILETTE;
      }
    }
    vTaskDelay(15);
  }
}

void TaskActions(void *pvParameters)  // This is a task.
{
  for(;;) {
    switch (etats)
    {
    case PLUIE:
      suspendDemo();
      digitalWrite(3, HIGH);
      for (size_t i = 0; i < 10; i++)
      {
        blink(7, 500);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      digitalWrite(3, LOW);
      break;

    case ORAGE:
      suspendDemo();
      digitalWrite(4, HIGH);
      for (size_t i = 0; i < 10; i++)
      {
        blink(7, 200);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      digitalWrite(4, LOW);
      break;

    case TOILETTE:
      suspendDemo();
      digitalWrite(5, HIGH);
      for (size_t i = 0; i < 10; i++)
      {
        blink(7, 800);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      digitalWrite(5, LOW);
      break;
    
    default:
      etats = IDLE;
      vTaskResume(xDemoHandle);
      break;
    }
    vTaskDelay(1);
  }
}
