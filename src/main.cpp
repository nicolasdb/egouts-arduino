#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>


// #include <fade.h>
#include <blink.h>
#include <mp3.h>

// define tasks for Demo, AnalogRead buttons, set Actions, MP3 lauch
void TaskDemo( void *pvParameters );
TaskHandle_t xDemoHandle = NULL;
void TaskButtons( void *pvParameters );
void TaskActions( void *pvParameters );
void TaskMP3storm( void *pvParameters );

// define différent scénario = les actions déclenchées par les boutons lorsque pressé.
enum {
  IDLE,
  PLUIE,
  ORAGE,
  S1,
  S2,
  S3,
  S4,
  S5,
  S6,
  S7
} etats;

// define MCP0
Adafruit_MCP23017 mcp0;


// the setup function runs once when you press reset or power the board
void setup() {

  // define all buttons
  mcp0.begin(0);      // use default address 0
  //  This could probably be an array
  for (size_t i = 0; i < 9 ; i++){
    mcp0.pinMode(i, OUTPUT);
    mcp0.digitalWrite(i, HIGH);
    delay(10);
  }




  // pin for action test
  pinMode(7, OUTPUT);
  // reading buttons
  pinMode(A1, INPUT);


  // fadeSetup();  // mcp0 setup
// initialize mp3setup
  mp3setup();

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
   digitalWrite(7, LOW);
  // fadeOFF();
}


/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDemo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) {
    // Si on est en IDLE on joue la démo
for (size_t i = 0 ; i < 9 ; i++){
  mcp0.digitalWrite(i, HIGH);
  delay(10);
  }
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
for (size_t i = 0 ; i < 9 ; i++){
  mcp0.digitalWrite(i, LOW);
  delay(10);
  }
  vTaskDelay( delay / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskButtons(void *pvParameters)
{
  (void) pvParameters;

  for (;;) {

    if (etats == IDLE) {
      int buttonValue = analogRead(A1);
      Serial.println("Waiting entry");
      if (buttonValue > 1000) {
        Serial.println("Button 1");
        etats = PLUIE;
      } else if (buttonValue > 900 && buttonValue < 990) {
        Serial.println("Button 2");
        etats = ORAGE;
      } else if (buttonValue > 780 && buttonValue < 890) {
        Serial.println("Button 3");
        etats = S1;
      } else if (buttonValue > 670 && buttonValue < 770) {
        Serial.println("Button 4");
        etats = S2;
      } else if (buttonValue > 550 && buttonValue < 650) {
        Serial.println("Button 5");
        etats = S3;
      } else if (buttonValue > 440 && buttonValue < 540) {
        Serial.println("Button 6");
        etats = S4;
      } else if (buttonValue > 330 && buttonValue < 430) {
        Serial.println("Button 7");
        etats = S5;
      } else if (buttonValue > 210 && buttonValue < 320) {
        Serial.println("Button 8");
        etats = S6;
      } else if (buttonValue > 100 && buttonValue < 200) {
        Serial.println("Button 9");
        etats = S7;
    }
    }
    vTaskDelay(10);
  }
}

void TaskActions(void *pvParameters)  // This is a task.
{
  for(;;) {
    switch (etats)
    {
    case PLUIE:
      suspendDemo();
      for (size_t i = 0; i < 10; i++)
      {
        blink(7, 500);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      // digitalWrite(3, LOW);
      break;

    case ORAGE:
      suspendDemo();
      // digitalWrite(4, HIGH);
      mp3(2);
      // for (size_t i = 0; i < 10; i++)
      // {
      //   blink(7, 200);
      // }
      // vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      // digitalWrite(4, LOW);
      break;

    case S1:
      suspendDemo();
      // digitalWrite(5, HIGH);
      // for (size_t i = 0; i < 5; i++)
      // {
      //   blink(7, 800);
      // }
      // vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      // digitalWrite(5, LOW);
      break;

    default:
      etats = IDLE;
      vTaskResume(xDemoHandle);
      break;
    }
    vTaskDelay(1);
  }
}
