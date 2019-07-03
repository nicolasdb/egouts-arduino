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
  S1,
  S2,
  S3,
  S4,
  S5,
  S6,S7,S8,S9
} etats;
int buttonID = 0;

// define MCP0
Adafruit_MCP23017 mcp0;
int count = 0;  // compteur pour scanner led k2000
int speed = 1;  // delay pour scanner led k2000
int ledButtonArray[] = {1,2,3,4,5,6,7,8,9};

// the setup function runs once when you press reset or power the board
void setup() {

  // define all buttons
  mcp0.begin(0);      // use default address 0
  //  This could probably be an array
  for (size_t i = 0; i < 9 ; i++){
    mcp0.pinMode(ledButtonArray[i], OUTPUT);
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
  for (size_t i = 0 ; i < 9 ; i++){
    mcp0.digitalWrite(ledButtonArray[i], LOW);
    delay(10);
    }
}



/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDemo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) {    // Si on est en IDLE on joue la démo
    for (count = 0 ; count < 9 ; count++){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed);
      mcp0.digitalWrite(ledButtonArray[count+1], HIGH);
      vTaskDelay(speed);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay(speed*2);
    }
    // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
    for (count = 9 ; count > 0 ; count--){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed);
      mcp0.digitalWrite(ledButtonArray[count-1], HIGH);
      vTaskDelay(speed);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay(speed*2);
    }
    // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
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
        buttonID = 1;
        etats = S1;
      } else if (buttonValue > 900 && buttonValue < 990) {
        Serial.println("Button 2");
        buttonID = 2;
        etats = S2;
      } else if (buttonValue > 780 && buttonValue < 890) {
        Serial.println("Button 3");
        buttonID = 3;
        etats = S3;
      } else if (buttonValue > 670 && buttonValue < 770) {
        Serial.println("Button 4");
        buttonID = 4;
        etats = S4;
      } else if (buttonValue > 550 && buttonValue < 650) {
        Serial.println("Button 5");
        buttonID = 5;
        etats = S5;
      } else if (buttonValue > 440 && buttonValue < 540) {
        Serial.println("Button 6");
        buttonID = 6;
        etats = S6;
      } else if (buttonValue > 330 && buttonValue < 430) {
        Serial.println("Button 7");
        buttonID = 7;
        etats = S7;
      } else if (buttonValue > 210 && buttonValue < 320) {
        Serial.println("Button 8");
        buttonID = 8;
        etats = S8;
      } else if (buttonValue > 100 && buttonValue < 200) {
        Serial.println("Button 9");
        buttonID = 9;
        etats = S9;
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
    case S1:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      blink(7, 200);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      mp3(1);
      Serial.println("Pluie");
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
      Serial.println("Champs");
      Serial.println("et Jardin");
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
      Serial.println("Nappe Phréatique");
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
      etats = IDLE;
      mcp0.digitalWrite(buttonID, LOW);
      break;

    case S2:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      Serial.println("Pluie");
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
      for (size_t i = 0; i < 10; i++)
      {
        blink(7, 100);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      mcp0.digitalWrite(buttonID, LOW);
      break;

    case S3:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      for (size_t i = 0; i < 3; i++)
      {
        blink(7, 250);
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      etats = IDLE;
      mcp0.digitalWrite(buttonID, LOW);
      break;

    case S4:
        suspendDemo();
        mcp0.digitalWrite(buttonID, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
        etats = IDLE;
        mcp0.digitalWrite(buttonID, LOW);
        break;

    case S5:
            suspendDemo();
            mcp0.digitalWrite(buttonID, HIGH);
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
            etats = IDLE;
            mcp0.digitalWrite(buttonID, LOW);
            break;
    case S6:
            suspendDemo();
            mcp0.digitalWrite(buttonID, HIGH);
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
            etats = IDLE;
            mcp0.digitalWrite(buttonID, LOW);
            break;
    case S7:
            suspendDemo();
            mcp0.digitalWrite(buttonID, HIGH);
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
            etats = IDLE;
            mcp0.digitalWrite(buttonID, LOW);
            break;
    case S8:
            suspendDemo();
            mcp0.digitalWrite(buttonID, HIGH);
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
            etats = IDLE;
            mcp0.digitalWrite(buttonID, LOW);
            break;
    case S9:
            suspendDemo();
            mcp0.digitalWrite(buttonID, HIGH);
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
            etats = IDLE;
            mcp0.digitalWrite(buttonID, LOW);
            break;




    default:
      buttonID = 0;
      etats = IDLE;
      vTaskResume(xDemoHandle);
      break;
    }
    vTaskDelay(1);
  }
}
