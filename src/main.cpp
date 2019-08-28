#include <Arduino.h>
#include <Arduino_FreeRTOS.h>         // Multitasking
#include <SoftwareSerial.h>           // controle du module mp3
#include <DFRobotDFPlayerMini.h>      // controle du module mp3
#include <Wire.h>                     // controle des MCP
#include <Adafruit_MCP23017.h>        // controle des MCP

#include <mp3.h>                      // controle storm du module mp3

// define tasks for Demo, AnalogRead buttons, set Actions, MP3 launch
void TaskDemo( void *pvParameters );          // define demo
TaskHandle_t xDemoHandle = NULL;              // nécessaire pour activer la pause et reprise de la tâche
void TaskButtons( void *pvParameters );       // define Buttons, pour l'écoute de quel bouton est activé
void TaskActions( void *pvParameters );       // define la tâche d'actions.
void TaskPump( void *pvParameters );

// define différent scénario = les actions déclenchées par les boutons lorsque pressé.
enum {
  IDLE,
  S1,S2,S3,S4,S5,S6,S7,S8
} etats;
int buttonID = 0;



// define MCP0 - registre pour les leds bouttons "scénario"
Adafruit_MCP23017 mcp0;
// define MCP1 - registre pour les electrovannes via relais et options
Adafruit_MCP23017 mcp1;

int ledButtonArray[] = {7,6,9,8,11,10,5,4};   // liste les pin utilisées sur mcp0 et leur ordre
int count = 0;                                // compteur pour scanner led k2000
int speed = 20;                               // delay pour scanner led k2000

// bool reedValue;
// bool pumpStat;
// int pump = 0;

// mapping valveArray, liste les pin utilisées sur mcp1
// int valveArray[] = {0,1,2,3,4,5,6,7};
// int valveBrray[] = {8,9,10,11,12,13,14,15};

// define n° relay for valves on MCP0
int pump = 0;         //pompe 220V
int ledBO1 = 1;       //led du bassin d'orage haut
int citerne = 2;      //valve 1 citerne
int pJardin = 3;      //valve 3 pluie jardin
int pChamp = 4;       //valve 8 pluie champ
int tImper = 5;       //valve 5 pluie toiture G
int tPlant = 6;       //valve 9 pluie toiture D
int riviere = 7;      //valve 10 rivière
int ledBO2 = 8;       //led du bassin d'orage bas
int ledCiterne = 9;   //led de la citerne
int sImper = 13;       //valve 12 rue&place
int collecteur = 11;  //valve 7 collecteur maison (oeuf)
int bOrage = 12;      //valve 4 bassin d'orage
int simVille1 = 10;   //valve 11 activité voisine
int egout = 14;       //valve 2 egout champignon
int sterput = 15;     //valve 6 sterput maison

// mapping leds
// int ledMurray[] = {1,2,3,4,5,6,7,8,9,10};
int nappe = 7;          // nappe Phréatique 12V
int egg = 8;            // collecteur oeuf 12V
int sdb1 = 2;           //ok
int toilette = 5;
int cuisine = 6;
int arrosage = 3;
int lavelinge = 1;
int cave = 4;

/*--------------------------------------------------*/
/*---------------------- setup ---------------------*/
/*--------------------------------------------------*/
// setup function runs once when you press reset or power the board

void setup() {
  mp3setup();    // initialize mp3setup
  // Serial.begin(115200);

  // INPUT sensors
  pinMode(A3, INPUT);     // input pin buttons scénario

  for (size_t i = 1; i < 9 ; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i,LOW);
    delay(10);
  }
// digitalWrite(1,HIGH);

  // MCP0, define all buttons
  mcp0.begin(0);                               // use default address 0
  for (size_t i = 0; i < 8 ; i++){
    mcp0.pinMode(ledButtonArray[i], OUTPUT);
    delay(50);
  }

  mcp1.begin(1);                               // use default address 1
  for (int j = 0; j < 16 ; j++){
    mcp1.pinMode(j, OUTPUT);
    mcp1.digitalWrite(j, LOW);
    delay(50);
  }

  // Serial.println("j'ai finis");
  etats = IDLE;

  // and at last,
  // set up tasks to run independently and their priority.
  xTaskCreate(
    TaskDemo
    ,  (const portCHAR *)"Demo"       // A name just for humans
    ,  128                            // This stack size can be checked & adjusted
                                      // by reading the Stack Highwater
    ,  NULL
    ,  1                              // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &xDemoHandle );

  xTaskCreate(
    TaskButtons
    ,  (const portCHAR *)"Buttons"    // A name just for humans
    ,  128                            // This stack size can be checked & adjusted
                                      // by reading the Stack Highwater
    ,  NULL
    ,  3                              // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskActions
    ,  (const portCHAR *)"Actions"    // A name just for humans
    ,  128                            // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2                              // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

}

/*--------------------------------------------------*/
/*---------------------- loop ----------------------*/
/*--------------------------------------------------*/
// the loop function is not use here, instead we use FreeRTOS's tasks
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
    for (count = 2; count < 6 ; count++){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count+1], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay((speed*5) / portTICK_PERIOD_MS);
    }
    // vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
    for (count = 6 ; count > 2 ; count--){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count-1], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay((speed*5) / portTICK_PERIOD_MS);
    }
  }
}


/*---------------------- buttons ---------------------*/
void TaskButtons(void *pvParameters)
{
  (void) pvParameters;

  for (;;) {

    if (etats == IDLE) {
      int buttonValue=analogRead(A3);
      Serial.print("Waiting entry");
      Serial.println(buttonValue);
      if (buttonValue>950) {
        Serial.println("Button 8");
        buttonID = ledButtonArray[8-1];
        etats = IDLE;
      } else if (buttonValue > 850 && buttonValue < 949) {
        Serial.println("Button 7");
        buttonID = ledButtonArray[7-1];
        etats = S5;
      } else if (buttonValue > 700 && buttonValue < 849) {
        Serial.println("Button 6");
        buttonID = ledButtonArray[6-1];
        etats = S4;
      } else if (buttonValue > 600 && buttonValue < 699) {
        Serial.println("Button 5");
        buttonID = ledButtonArray[5-1];
        etats = S3;
      } else if (buttonValue > 460 && buttonValue < 599) {
        Serial.println("Button 4");
        buttonID = ledButtonArray[4-1];
        etats = S2;
      } else if (buttonValue > 310 && buttonValue < 459) {
        Serial.println("Button 3");
        buttonID = ledButtonArray[3-1];
        etats = S1;
      } else if (buttonValue > 210 && buttonValue < 309) {
        Serial.println("Button 2");
        buttonID = ledButtonArray[2-1];
        etats = IDLE;
      } else if (buttonValue > 110 && buttonValue < 209) {
        Serial.println("Button 1");
        buttonID = ledButtonArray[1-1];
        etats = IDLE;
      }
    }
    vTaskDelay(10);
  }
}

/*---------------------- actions ---------------------*/
void TaskActions(void *pvParameters)  // This is a task.
{
  for(;;) {
    switch (etats)
    {
    case IDLE:
      buttonID = 0;
      etats = IDLE;
      vTaskResume(xDemoHandle);
      break;


    case S1:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second

      // Serial.println("test mcp1 pin0");
      mcp1.digitalWrite(pump, HIGH);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp1.digitalWrite(pJardin, HIGH);
      digitalWrite(nappe, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
      mcp1.digitalWrite(pJardin, LOW);
      mcp1.digitalWrite(pump, LOW);
      digitalWrite(nappe, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      // Serial.println("lavelinge");
      // piste2();
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
      break;

    case S2:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second

      // Serial.println("test mcp1 pin1");
      mcp1.digitalWrite(pump, HIGH);
      mcp1.digitalWrite(tImper, HIGH);
      digitalWrite(egg, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
      mcp1.digitalWrite(tImper, LOW);
      mcp1.digitalWrite(pump, LOW);
      digitalWrite(egg, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      // Serial.println("wc");
      // piste2();
        // vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
    break;

    case S3:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second

      // Serial.println("test mcp1 pin2");
      mcp1.digitalWrite(pump, HIGH);
      mcp1.digitalWrite(sImper, HIGH);
      digitalWrite(sdb1, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
      mcp1.digitalWrite(sImper, LOW);
      digitalWrite(sdb1, LOW);
      mcp1.digitalWrite(pump, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      // Serial.println("robinet");
      // piste3();
      //   vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
    break;

    case S4:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      // Serial.println("test mcp1 pin3");

      // mcp1.digitalWrite(pump, HIGH);
      mcp1.digitalWrite(riviere, HIGH);
      digitalWrite(toilette, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );

      mcp1.digitalWrite(riviere, LOW);
      digitalWrite(toilette, LOW);
      // mcp1.digitalWrite(pump, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      // Serial.println("pluie");
      // piste4();
        // vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
    break;

    case S5:              /*---------------------- case ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      // Serial.println("test storm");
      // storm(1);
      // piste4();
      // mcp1.digitalWrite(pump, HIGH);
      mcp1.digitalWrite(3, HIGH);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(4, HIGH);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(egout, HIGH);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(bOrage, HIGH);
      // digitalWrite(cuisine, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
      mcp1.digitalWrite(3, LOW);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(4, LOW);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(egout, LOW);
      vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp1.digitalWrite(bOrage, LOW);
      // mcp1.digitalWrite(pump, LOW);

      // digitalWrite(cuisine, LOW);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );

      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
    break;
    case S6:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
      break;
    case S7:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
      break;
    case S8:
      suspendDemo();
      mcp0.digitalWrite(buttonID, HIGH);
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      mcp0.digitalWrite(buttonID, LOW);
      etats = IDLE;
      break;



    }
    vTaskDelay(1);
  }
}
