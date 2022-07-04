#include <Arduino.h>
#include <Arduino_FreeRTOS.h>         // Multitasking. Attention version 10.1.1-1!!
#include <SoftwareSerial.h>           // controle du module mp3
#include <DFRobotDFPlayerMini.h>      // controle du module mp3
#include <Wire.h>                     // controle des MCP
#include <Adafruit_MCP23017.h>        // controle des MCP

#define portCHAR char                 // resolve error: expected primary-expression before 'const'

// define tasks for Demo, AnalogRead buttons, set Actions, MP3 launch
void TaskDemo( void *pvParameters );          // define demo
TaskHandle_t xDemoHandle = NULL;              // nécessaire pour activer la pause et reprise de la tâche
void TaskButtons( void *pvParameters );       // define Buttons, pour l'écoute de quel bouton est activé
void TaskActions( void *pvParameters );       // define la tâche d'actions.


#pragma region        // variables

// define différent scénario = les actions déclenchées par les boutons lorsque pressé.
enum {
  IDLE,
  S1,S2,S3,S4,S5
} etats;
int buttonID = 0;

// mp3 module variable
int rxPin = 10;      // DFplayer TX to Arduino RX pin 10
int txPin = 11;      // DFplayer RX to Arduino TX pin 11
int busyPin = 12;    // DFplayer BUSY connected to pin 12

SoftwareSerial mySoftwareSerial(rxPin, txPin);
DFRobotDFPlayerMini stormPlayer;

// define MCP0 - registre pour les leds bouttons "scénario"
Adafruit_MCP23017 mcp0;
// define MCP1 - registre pour les electrovannes via relais et options
Adafruit_MCP23017 mcp1;

int ledButtonArray[] = {0,1,2,3,4};           // liste les pin utilisées sur mcp0
int count = 0;                                // compteur pour scanner led k2000
int speed = 20;                               // delay pour scanner led k2000

// define n° relay for valves on MCP1
int pump = 0;           //pompe 220V
int ledCiterne = 1;     //citerne 5V
int citerne = 2;        //valve 1 citerne
int pJardin = 3;        //valve 3 pluie jardin
int pChamp = 4;         //valve 8 pluie champ
int tImper = 5;         //valve 5 pluie toiture G
int tPlant = 6;         //valve 9 pluie toiture D
int simVille = 11 ;     //valve 11 ville
int ledBO2 = 8;         //bassin d'orage bas 5V
int ledBO1 = 9;         //bassin d'orage haut 5V
int collecteur = 10;    //valve 7  collecteur maison (oeuf)
int ruePlace = 7;       //valve 12 rue
int bOrage = 12;        //valve 4 bassin d'orage
int egout = 13;         //valve 2 egout champignon
int riviere = 14;       //valve 10 rivière
int sterput = 15;       //valve 6 sterput maison

// mapping leds
int StripPin = 5;        // MCP0 - LED strip (via MOSFET) 12V !!!
int nappe = 6;           // MCP0 - nappe Phréatique 12V !!!
int egg = 7;             // MCP0 - collecteur oeuf 12V !!!
int sdb1 = 8;            // MCP0 - home 1
int toilette = 9;        // MCP0 - home 2
int cuisine = 10;        // MCP0 - home 3
int arrosage = 11;       // MCP0 - home 4
int lavelinge = 12;      // MCP0 - home 5
int cave = 13;           // MCP0 - home 6


#pragma endregion	

/*--------------------------------------------------*/
/*---------------------- setup ---------------------*/
/*--------------------------------------------------*/
// setup function runs once when you press reset or power the board

void setup() {

  // INPUT sensors
  pinMode(A3, INPUT);           // input pin buttons scénario
  pinMode(busyPin, INPUT);      // LOW while playing
  pinMode(A0, INPUT);           // for random seed

  // clean start LOW arduino pin 2 to 9 
  for (size_t i = 2; i <= 9 ; i++){
    mcp0.pinMode(i, OUTPUT);
    digitalWrite(i,LOW);
    delay(10);
  }

  // MCP0, define all OUTPUT
  mcp0.begin(0);                               // use default address 0 - GND/GND/GND
  for (int i = 0; i < 16 ; i++){
    mcp0.pinMode(i, OUTPUT);
    mcp0.digitalWrite(i, LOW);
    delay(50);
  }

  // MCP1, define all OUTPUT
  mcp1.begin(1);                               // use default address 1 - 5V/GND/GND
  for (int j = 0; j < 16 ; j++){
    mcp1.pinMode(j, OUTPUT);
    mcp1.digitalWrite(j, LOW);
    delay(50);
  }

  delay(1000);       // wait a bit before turning on the mp3module

  // MP3 module, define
   mySoftwareSerial.begin(9600);
  //  Serial.begin(115200);                     // uncomment to activate com with PC, 9600 is used for com with DFplayer
   Serial.println(F("Initializing DFPlayer..."));
   randomSeed(analogRead(A0));                  // random seed from analog unused pin.

   if (!stormPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
     Serial.println(F("Unable to begin. Check connection and SD card, or reset the Arduino."));
     while (true);
   }

   Serial.println(F("stormPlayer Mini online."));

   stormPlayer.setTimeOut(500);                   // Set serial communictaion time out 500ms
   stormPlayer.volume(29);                        // Set volume value (0~30).
   stormPlayer.EQ(DFPLAYER_EQ_NORMAL);              // Set EQ to BASS (normal/pop/rock/jazz/classic/bass)
   stormPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Set device we use SD as default


  // Serial.println("setup done");
  etats = IDLE;

  // and at last,
  // set up freeRTOS tasks to run independently and their priority.
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
  for (size_t i = 0 ; i <= 15 ; i++){
    mcp0.digitalWrite(i, LOW);
    vTaskDelay(1);
    }
}

void storm (uint32_t stormCount) {
    Serial.print("Boucles : ");
    Serial.println(stormCount);
for (size_t i=1 ; i<= stormCount ; i++){
    uint32_t flashCount = random (10, 15);        // Min. and max. number of flashes each loop
      // Brightness not used because MCP23017 don't support PWM
      // int flashBrightnessMin =  10;           // LED flash min. brightness (0-255)  
      // int flashBrightnessMax =  255;          // LED flash max. brightness (0-255)

      int flashDurationMin = 1;               // Min. duration of each seperate flash
      int flashDurationMax = 50;              // Max. duration of each seperate flash

      int nextFlashDelayMin = 1;              // Min, delay between each flash and the next
      int nextFlashDelayMax = 150;            // Max, delay between each flash and the next

      int thunderDelay = random (300, 1000);  // Min. and max. delay between flashing and playing sound
      int thunderFile = random (1, 7);        // There are 7 soundfiles: 001.mp3 ... 07.mp3
      int loopDelay = random (50, 200);       // Min. and max. delay between each loop

      Serial.println();
      Serial.print(F("Flashing, count: "));
      Serial.println( flashCount );

      for (uint32_t flash = 0 ; flash <= flashCount; flash += 1) {  // Flashing LED strip in a loop, random count

        mcp0.digitalWrite(StripPin, HIGH);                          // Turn LED strip on, random brightness
        delay(random(flashDurationMin, flashDurationMax));          // Keep it tured on, random duration

        mcp0.digitalWrite(StripPin, LOW);                           // Turn the LED strip off
        delay(random(nextFlashDelayMin, nextFlashDelayMax));        // Random delay before next flash
      }

      Serial.print(F("Pausing before playing thunder sound, milliseconds: "));
      Serial.println(thunderDelay);
      delay(thunderDelay);

      Serial.print(F("Playing thunder sound, file number: "));
      Serial.println(thunderFile);
      stormPlayer.playFolder(1, thunderFile);
      delay(1000); // Give the DFPlayer some time

      while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
      }

      Serial.print(F("Pausing before next loop, milliseconds: "));
      Serial.println(loopDelay);
      delay(loopDelay);

  }
}

void piste1 () {
  stormPlayer.playFolder(2,1);        // lavelinge 26'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  // }
}

void piste2 () {
  stormPlayer.playFolder(2,2);        // wc 10'
  delay(1000); // Give the DFPlayer some time
}

void piste3 () {
  stormPlayer.playFolder(2,3);        // robinet 20'
  delay(1000); // Give the DFPlayer some time
}

void piste4 () {
  stormPlayer.playFolder(2,4);        // pluie 24'
  delay(1000); // Give the DFPlayer some time
}

void piste5 () {
  stormPlayer.playFolder(2,5);        // douche 24'
  delay(1000); // Give the DFPlayer some time
}



/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

/*---------------------- demo ---------------------*/
void TaskDemo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) {    // on joue la démo indéfiniment : effet scanner k2000
      
    for (count = 0; count <= 3 ; count++){
      mcp0.digitalWrite(ledButtonArray[count], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count+1], HIGH);
      vTaskDelay(speed / portTICK_PERIOD_MS);
      mcp0.digitalWrite(ledButtonArray[count], LOW);
      vTaskDelay((speed*5) / portTICK_PERIOD_MS);
    }
    for (count = 4 ; count >= 1 ; count--){
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
      int buttonValue=analogRead(A3);                   // First reading for soft debounce
      delay(20);
      int buttonValue2=analogRead(A3);                  // Second reading for soft debounce                                      
      Serial.print("Waiting entry");
      Serial.println(buttonValue);
      if (buttonValue >= 950 && buttonValue2 >= 950){                                                             // 10R read: 1021
        Serial.println("Button 1");
        buttonID = 1;
        // etats = IDLE;
        etats = S1;
      } else if (buttonValue >= 876 && buttonValue < 949 && buttonValue2 >= 876 && buttonValue2 < 949) {          // 10K read: 930
        Serial.println("Button 2");
        buttonID = 2;
        // etats = IDLE;
        etats = S2;
      } else if (buttonValue >= 751 && buttonValue < 875 && buttonValue2 >= 751 && buttonValue2 < 875) {          // 22K read: 840
        Serial.println("Button 3");
        buttonID = 3;
        // etats = IDLE;
        etats = S3;
      } else if (buttonValue >= 351 && buttonValue < 750 && buttonValue2 >= 351 && buttonValue2 < 750) {          // 56K read: 659
        Serial.println("Button 4");
        buttonID = 4;
        // etats = IDLE;
        etats = S4;
      } else if (buttonValue >= 150 && buttonValue < 350 && buttonValue2 >= 150 && buttonValue2 < 350) {          // 210K read: 280
        Serial.println("Button 5");
        buttonID = 5;
        // etats = IDLE;
        etats = S5;
      } 
    }
    vTaskDelay(1);
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





    case S1:              /*---------------------- case Pluie ---------------------*/
    // eau de pluie, alimentation de la nappe Phréatique

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS );    // wait 

// start scénario ---------------------------------------------------------------------------------------------------------
      
      mcp1.digitalWrite(pump, HIGH);                          // start pompe
        piste4();                                             // activation audio pluie 24'

      mcp1.digitalWrite(pChamp, HIGH);                        // start pluie sur les champs et 
      mcp1.digitalWrite(pJardin, HIGH);                       // les surfaces perméables
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );       // Waiting x second
      mcp0.digitalWrite(nappe, HIGH);                         // turn on led nappe Phréatique
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
      mcp1.digitalWrite(pChamp, LOW);                         // stop pluie
      mcp1.digitalWrite(pJardin, LOW);                        // stop pluie
      mcp1.digitalWrite(pump, LOW);                           // stop pompe
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );

      // on attend un peu pour l'histoire de la nappe Phréatique
      mcp0.digitalWrite(nappe, LOW);                          // turn off led nappe
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

// end scénario ---------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
      break;





    case S2:              /*---------------------- case Eau potable ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 1/2 second
// start scénario
        // captation eau potable
        mcp0.digitalWrite(nappe, HIGH);      // turn on nappe Phréatique
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
        // on pompe dans la nappe, ou dans la rivière
        mcp0.digitalWrite(nappe, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
          // Waiting pompage, chateau d'eau
        mcp0.digitalWrite(cuisine, HIGH);  // turn on led cuisine
        piste3(); // piste audio robinet 20'
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
        //waiting vaisselle, ambiance cuisine
        // turn on pompe et activation du sterput, + led Cave.
        mcp1.digitalWrite(pump, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
        // waiting action robinet en cuisine
        // puis turn on sterput à la cave + led
        mcp1.digitalWrite(sterput, HIGH);
        mcp0.digitalWrite(cave, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );
        // Waiting trajet de l'eau jusqu'au collecteur rue
        mcp1.digitalWrite(collecteur, HIGH);
        mcp0.digitalWrite(egg, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
          // Waiting end audio robinet
        mcp0.digitalWrite(cuisine, LOW); // turn of led cuisine
          // Waiting et turn off sertup et led cave
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
        mcp1.digitalWrite(sterput, LOW);
        mcp0.digitalWrite(cave, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
        mcp1.digitalWrite(collecteur, LOW);
        mcp0.digitalWrite(egg, LOW);
        mcp1.digitalWrite(pump, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );

// end scénario
      mcp0.digitalWrite(buttonID-1, LOW);
      // vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
      etats = IDLE;
    break;





    case S3:              /*---------------------- case Épuration ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 1/2 second
// start scénario

mcp0.digitalWrite(sdb1, HIGH); // turn on sdb
piste5();  //piste audio douche 24'
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
  //wait douche, ajout des autres pièces d'eau.
mcp1.digitalWrite(pump, HIGH);  // turn on pompe
    vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
mcp1.digitalWrite(sterput, HIGH);
mcp0.digitalWrite(cave, HIGH);
  // eau dans la chambre de visite
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
mcp0.digitalWrite(cuisine, HIGH);
mcp0.digitalWrite(lavelinge, HIGH); // turn on lavelinge

  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 4 );

  //waiting fin des actions maisons
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
  //waiting, l'eau arrive au collecteur rue (egg)
mcp1.digitalWrite(collecteur, HIGH);   //collecteur oeuf + led
mcp0.digitalWrite(egg, HIGH);
mcp0.digitalWrite(sdb1, LOW);
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
mcp0.digitalWrite(lavelinge, LOW);
mcp0.digitalWrite(cuisine, LOW);
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
  mcp1.digitalWrite(sterput, LOW);
  mcp0.digitalWrite(cave, LOW);
mcp1.digitalWrite(bOrage, HIGH);  // l'eau monte un peu
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
  //waiting, l'eau arrive au champignon
mcp1.digitalWrite(bOrage, LOW);  // on arrête l'eau au bassin
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );
  //waiting, l'eau transit jusqu'à la station d'épuration.
mcp1.digitalWrite(riviere, HIGH);
  vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );
  mcp1.digitalWrite(collecteur, LOW);   //collecteur oeuf + led
  mcp0.digitalWrite(egg, LOW);
  //waiting, eau à la rivière
mcp1.digitalWrite(riviere, LOW);
mcp1.digitalWrite(pump, LOW);

// end scénario
      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;





    case S4:              /*---------------------- case Orage ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 1/2 second
// start scénario
        storm(1); // start Orage
        piste4(); // start audio pluie 24'
        mcp1.digitalWrite(pump, HIGH);  // turn on pompe
            vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
        mcp1.digitalWrite(tImper, HIGH);      // pluie toitures maison
        mcp1.digitalWrite(tPlant, HIGH);
        mcp1.digitalWrite(pJardin, HIGH);
        mcp1.digitalWrite(pChamp, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
          // Waiting, il pleut
        mcp1.digitalWrite(ruePlace, HIGH);    // eau sur rue&place
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
          // Waiting, l'eau coule sur la rue et la place
        mcp1.digitalWrite(tImper, LOW);     // turn off pluie
        mcp1.digitalWrite(tPlant, LOW);
        mcp1.digitalWrite(pJardin, LOW);
        mcp1.digitalWrite(pChamp, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
          // Waiting, la pluie s'arrête mais l'eau arrive dans le collecteur
        mcp1.digitalWrite(collecteur, HIGH);   //collecteur oeuf + led
        mcp0.digitalWrite(egg, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );
          // l'eau s'est écoulée des ruePlace
        mcp1.digitalWrite(ruePlace, LOW);   // turn off rue&place
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
          // waiting, l'eau est dans le champignon, bOrage + led Haut + egout
        mcp1.digitalWrite(bOrage, HIGH);  // démo trop plein vers BassinOrage
        mcp1.digitalWrite(ledBO1, HIGH);
        mcp1.digitalWrite(egout, HIGH);   // l'eau s'écoule dans le champignon
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 16 );
          // waiting, turn off egg, l'orage est fini
        mcp1.digitalWrite(collecteur, LOW);
        mcp0.digitalWrite(egg, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
          //le trop plein s'est déversé dans le bassin, led bas + egout
        mcp1.digitalWrite(bOrage, LOW);
        mcp1.digitalWrite(ledBO1, LOW);
        mcp1.digitalWrite(ledBO2, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
          //waiting, vidange du BassinOrage
        mcp1.digitalWrite(ledBO2, LOW);
        mcp1.digitalWrite(egout, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
        mcp1.digitalWrite(pump, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );

// end scénario
      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;




    case S5:              /*---------------------- case Récup pluie ---------------------*/
      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for 1/2 second
// // start scénario

        piste4(); // piste audio pluie 24'
        mcp1.digitalWrite(pump, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
          // il pleut sur les toitures
        mcp1.digitalWrite(tImper, HIGH);
        mcp1.digitalWrite(tPlant, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
        // Waiting, une captation de l'eau vers la citerne
        mcp1.digitalWrite(citerne, HIGH);
        mcp1.digitalWrite(ledCiterne, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 13 );
        // Waiting, la citerne se remplit.
        mcp1.digitalWrite(tImper, LOW);
        mcp1.digitalWrite(tPlant, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );
        // Waiting, la citerne continue de se remplir et s'arrête.
        mcp1.digitalWrite(citerne, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

        piste1();  //piste audio lavelinge 26'
        // digitalWrite(lavelinge, HIGH);
        mcp0.digitalWrite(arrosage, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 8 );
          //wait lavelinge, ajout de la pompe d'arrosage.
        // digitalWrite(lavelinge, LOW); // turn off lavelinge
        mcp0.digitalWrite(toilette, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );

        // piste2(); // piste audio toilette 10'
        mcp1.digitalWrite(collecteur, HIGH);
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
        mcp0.digitalWrite(cave, HIGH);
        mcp0.digitalWrite(egg, HIGH);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );
        // la pompe d'arrosage est éteinte
        mcp0.digitalWrite(toilette, LOW);
        mcp0.digitalWrite(arrosage, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );
          mcp1.digitalWrite(collecteur, LOW);
          mcp1.digitalWrite(pump, LOW);
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
          mcp0.digitalWrite(cave, LOW);
          mcp0.digitalWrite(egg, LOW);
          // waiting a bit pour la citerne
        mcp1.digitalWrite(ledCiterne, LOW);

// // end scénario
      mcp0.digitalWrite(buttonID-1, LOW);
      // vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
      etats = IDLE;
    break;
 
    }
    vTaskDelay(1);
  }
}
