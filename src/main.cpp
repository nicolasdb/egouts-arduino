#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>


// #include <fade.h>
#include <blink.h>          // lib pour l'action de clignotement
#include <mp3.h>            // controle du module mp3

// define tasks for Demo, AnalogRead buttons, set Actions, MP3 launch
void TaskDemo( void *pvParameters );          // define demo
TaskHandle_t xDemoHandle = NULL;              // nécessaire pour activer la pause et reprise de la tâche
void TaskButtons( void *pvParameters );       // define Buttons, pour l'écoute de quel bouton est activé
void TaskActions( void *pvParameters );       // define la tâche d'actions.
void TaskPump( void *pvParameters );

// define différent scénario = les actions déclenchées par les boutons lorsque pressé.
enum {
  IDLE,
  S1,S2,S3,S4,S5,S6,S7
} etats;
int buttonID = 0;



// define MCP0 - registre pour les leds Buttons et options
Adafruit_MCP23017 mcp0;
// define MCP1 - registre pour les electrovannes via relais
Adafruit_MCP23017 mcp1;
// define MCP0 - registre pour les leds d'ambiance maquette
Adafruit_MCP23017 mcp2;

int count = 0;  // compteur pour scanner led k2000
int speed = 15;  // delay pour scanner led k2000
int ledButtonArray[] = {7,6,9,8,11,10,5,4};  // liste les pin utilisée sur mcp0 et leur ordre

int valveArray[] = {0,1,2,3,4,5,6,7};
int valveBrray[] = {8,9,10,11,12,13,14,15};
int ledMurray[] = {1,2,3,4,5,6,7,8,9,10};

bool reedValue;
bool pumpStat;

// mapping valveArray
int pump = 3;

int pChamp = 4;
int tImper = 5;
int tPlant = 6;
int pJardin = 7;
int sImper = 8;
int citerne = 9;
int egout = 10;
int collecteur = 11;
int bOrage = 12;
int riviere = 13;



// mapping ledMurray
int nappe = 1;
int robinet = 2;
int lavelinge = 3;
int toilette = 4;












/*--------------------------------------------------*/
/*---------------------- setup ---------------------*/
/*--------------------------------------------------*/
// the setup function runs once when you press reset or power the board

void setup() {

  Serial.begin(115200);

  // INPUT buttons
  pinMode(A3, INPUT);  // input pin buttons
  pinMode(8, INPUT);   // input pin WaterLevel (reed switch)

  // define all buttons
  mcp0.begin(0);      // use default address 0
  for (size_t i = 0; i < 8 ; i++){
    mcp0.pinMode(ledButtonArray[i], OUTPUT);
    delay(100);
  }

  mcp1.begin(1);      // use default address 1
  for (int j = 0; j < 8 ; j++){
    mcp1.pinMode(valveArray[j], OUTPUT);
    // mcp1.pinMode(valveBrray[j], OUTPUT);
    delay(500);
    mcp1.digitalWrite(valveArray[j], HIGH);
    // mcp1.digitalWrite(valveBrray[j], HIGH);
    Serial.println(valveArray[j]);
    delay(500);
    mcp1.digitalWrite(valveArray[j], LOW);
  }
  // mcp1.pinMode(valveArray[9], OUTPUT);
  // mcp1.digitalWrite(valveArray[9], HIGH);


  // mcp2.begin(2);      // use default address 2
  // for (size_t i = 0; i < 10 ; i++){
  //   mcp2.pinMode(ledMurray[i], OUTPUT);
  //   delay(10);
  // }

  mp3setup();    // initialize mp3setup
  Serial.println("j'ai finis");
  etats = IDLE;

  // Now set up tasks to run independently and their priority.
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

}

/*--------------------------------------------------*/
/*---------------------- loop ----------------------*/
/*--------------------------------------------------*/
// the loop function is not use here

void loop()
{
}

/*------------------------------------------------------*/
/*---------------------- functions ---------------------*/
/*------------------------------------------------------*/

// this function will suspend Demo and turn off all led buttons
void suspendDemo () {
  vTaskSuspend(xDemoHandle);
  for (size_t i = 0 ; i < 8 ; i++){
    mcp0.digitalWrite(ledButtonArray[i], LOW);
    vTaskDelay(1);
    }
}



/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

/*---------------------- demo ---------------------*/
void TaskDemo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) {    // on joue la démo en loop : effet scanner k2000
    for (count = 0 ; count < 8 ; count++){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count+1], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay((speed*5) / portTICK_PERIOD_MS);
    }
    // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
    for (count = 8 ; count > 0 ; count--){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count-1], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay((speed*5) / portTICK_PERIOD_MS);
    }
    // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
// if (pumpStat == 1) {
//   mcp2.digitalWrite(nappe, HIGH);
//   vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
// } else {mcp2.digitalWrite(nappe, LOW);
//         vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );}

  }
}

/*---------------------- Pompe ---------------------*/
// void TaskPump(void *pvParameters)  // This is a task.
// {
//   (void) pvParameters;
//   for (;;) {
//   if (reedStat == 1) {
    // mcp2.digitalWrite(nappe, HIGH);
    // vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
//                       }
//    else {
//     mcp2.digitalWrite(nappe, LOW);
//     vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
//         }
//     }
// }

/*---------------------- buttons ---------------------*/
void TaskButtons(void *pvParameters)
{
  (void) pvParameters;

  for (;;) {

    // reedValue = digitalRead(8);
    // Serial.println(reedValue);
    // pumpStat = reedValue;
    // vTaskDelay( (1000 / portTICK_PERIOD_MS)*0.5 );

    if (etats == IDLE) {
      int buttonValue=analogRead(A3);
      Serial.print("Waiting entry");
      Serial.println(buttonValue);
      if (buttonValue>910) {
        Serial.println("Button 8");
        buttonID = ledButtonArray[8-1];
        etats = IDLE;
      } else if (buttonValue > 800 && buttonValue < 909) {
        Serial.println("Button 7");
        buttonID = ledButtonArray[7-1];
        etats = S7;
      } else if (buttonValue > 700 && buttonValue < 799) {
        Serial.println("Button 6");
        buttonID = ledButtonArray[6-1];
        etats = S6;
      } else if (buttonValue > 540 && buttonValue < 699) {
        Serial.println("Button 5");
        buttonID = ledButtonArray[5-1];
        etats = S5;
      } else if (buttonValue > 420 && buttonValue < 539) {
        Serial.println("Button 4");
        buttonID = ledButtonArray[4-1];
        etats = S4;
      } else if (buttonValue > 310 && buttonValue < 419) {
        Serial.println("Button 3");
        buttonID = ledButtonArray[3-1];
        etats = S3;
      } else if (buttonValue > 210 && buttonValue < 309) {
        Serial.println("Button 2");
        buttonID = ledButtonArray[2-1];
        etats = S2;
      } else if (buttonValue > 110 && buttonValue < 209) {
        Serial.println("Button 1");
        buttonID = ledButtonArray[1-1];
        etats = S1;
      }
    }
    vTaskDelay(3);
  }
}

/*---------------------- actions ---------------------*/
void TaskActions(void *pvParameters)  // This is a task.
{
  for(;;) {
    switch (etats)
    {
    default:
      buttonID = 0;
      etats = IDLE;
      vTaskResume(xDemoHandle);
      break;

    // case IDLE:
    //
    //   buttonID = 0;
    //   etats = IDLE;
    //   vTaskResume(xDemoHandle);
    //
    //   break;

    case S1:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second

      Serial.println("Pluie Champs");
      Serial.println("Pluie Jardin");
      // mcp1.digitalWrite(pChamp, HIGH);
      // mcp1.digitalWrite(pJardin, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
      // mcp1.digitalWrite(pChamp, LOW);
      // mcp1.digitalWrite(pJardin, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      Serial.println("Nappe Phréatique");
      mcp2.digitalWrite(nappe, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
      mcp2.digitalWrite(nappe, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
      break;

    case S2:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);

      Serial.println("Pompage nappe Phréatique");
      mcp2.digitalWrite(nappe, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp2.digitalWrite(nappe, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      Serial.println("l'eau coule du robinet à la cuisine");
      mcp2.digitalWrite(robinet, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
      mcp2.digitalWrite(robinet, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      etats = IDLE;
      mcp0.digitalWrite(buttonID, LOW);
      break;

    case S3:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);

      Serial.println("il pleut sur la toiture imperméable");
      // mcp1.digitalWrite(tImper, HIGH);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      // mcp1.digitalWrite(tImper, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      // Serial.print("il pleut sur la toiture plantée");
      // mcp1.digitalWrite(tPlant, HIGH);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
      // mcp1.digitalWrite(tPlant, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      // Serial.println("la citerne se remplit et stock l'eau pour la canicule");
      // mcp1.digitalWrite(citerne, HIGH);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS)*2 );
      // mcp1.digitalWrite(citerne, LOW);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS)*1 );
      //
      // Serial.println("l'eau est utilisée pour la lessive");
      // mcp2.digitalWrite(lavelinge, HIGH);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS)*2 );
      // mcp2.digitalWrite(toilette, HIGH);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS)*2 );
      // mcp2.digitalWrite(lavelinge, LOW);
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS)*1 );
      // mcp2.digitalWrite(toilette, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      etats = IDLE;
      mcp0.digitalWrite(buttonID, LOW);
      break;

    case S4:              /*---------------------- case ---------------------*/
        suspendDemo();
        mcp0.digitalWrite(buttonID, HIGH);

        // for (count = 0 ; count < 14 ; count++){
        //   mcp1.digitalWrite(valveArray[count], HIGH);
        //   vTaskDelay(200 / portTICK_PERIOD_MS);
        //   mcp1.digitalWrite(valveArray[count+1], HIGH);
        //   vTaskDelay(200 / portTICK_PERIOD_MS);
        //   mcp1.digitalWrite(valveArray[count], LOW);
        //   vTaskDelay((200*5) / portTICK_PERIOD_MS);
        // }
        // // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
        // for (count = 14 ; count > 0 ; count--){
        //   mcp1.digitalWrite(valveArray[count], HIGH);
        //   vTaskDelay(200 / portTICK_PERIOD_MS);
        //   mcp1.digitalWrite(valveArray[count-1], HIGH);
        //   vTaskDelay(200 / portTICK_PERIOD_MS);
        //   mcp1.digitalWrite(valveArray[count], LOW);
        //   vTaskDelay((200*5) / portTICK_PERIOD_MS);
        // }

        vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
        etats = IDLE;
        mcp0.digitalWrite(buttonID, LOW);
        break;

    case S5:              /*---------------------- case ---------------------*/
        suspendDemo();
        mcp0.digitalWrite(buttonID, HIGH);

        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
        etats = IDLE;
        mcp0.digitalWrite(buttonID, LOW);
        break;
    case S6:              /*---------------------- case ---------------------*/
        suspendDemo();
        mcp0.digitalWrite(buttonID, HIGH);

        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
        etats = IDLE;
        mcp0.digitalWrite(buttonID, LOW);
        break;
    case S7:              /*---------------------- case ---------------------*/
        suspendDemo();
        mcp0.digitalWrite(buttonID, HIGH);

        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 0.5 );
        // mp3(1);
        etats = IDLE;
        mcp0.digitalWrite(buttonID, LOW);
        break;
    // case S8:
    //         suspendDemo();
    //         mcp0.digitalWrite(buttonID, HIGH);
    //         vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    //         etats = IDLE;
    //         mcp0.digitalWrite(buttonID, LOW);
    //         break;



    }
    vTaskDelay(1);
  }
}
