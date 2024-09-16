#include <Arduino.h>
#include <Arduino_FreeRTOS.h>         // Multitasking. Attention version 10.1.1-1!!
#include <SoftwareSerial.h>           // controle du module mp3
#include <DFRobotDFPlayerMini.h>      // controle du module mp3
#include <Wire.h>                     // controle des MCP
#include <Adafruit_MCP23X17.h>        // controle des MCP

#define portCHAR char                 // resolve error: expected primary-expression before 'const'

// define tasks for Demo, AnalogRead buttons, set Actions, MP3 launch
void TaskDemo( void *pvParameters );          // define demo
TaskHandle_t xDemoHandle = NULL;              // nécessaire pour activer la pause et reprise de la tâche
void TaskButtons( void *pvParameters );       // define Buttons, pour l'écoute de quel bouton est activé
void TaskActions( void *pvParameters );       // define la tâche d'actions.

// variables

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
Adafruit_MCP23X17 mcp0;
// define MCP1 - registre pour les electrovannes via relais et options
Adafruit_MCP23X17 mcp1;

int ledButtonArray[] = {0,1,2,3,4};           // liste les pin utilisées sur mcp0
int count = 0;                                // compteur pour scanner led k2000
int speed = 20;                               // delay pour scanner led k2000

// define n° relay for valves on MCP1 | n°relai>>n°valve
int pump = 0;         //pompe 220V
int ledCiterne = 1;   //citerne 5V
int sterput = 2;      //valve 1-- sterput maison
int citerne = 3;      //valve 2 citerne
int tImper = 4;       //valve 3 pluie toiture G
int tPlant = 5;       //valve 4 pluie toiture D
int egout = 6;        //valve 5 egout champignon
int bOrage = 7;       //valve 6 bassin d'orage
int ledBO2 = 8;       //bassin d'orage bas 5V
int ledBO1 = 9;       //bassin d'orage haut 5V
int pChamp = 10;      //valve 7 pluie champ
int pJardin = 11;     //valve 8 pluie jardin
int collecteur = 12;  //valve 9  collecteur maison (oeuf)
int ruePlace = 13;    //valve 10 rue et place
int riviere = 14;     //valve 11 rivière
int simVille = 15 ;   //valve 12 Maison voisine dans oeuf

// mapping leds
int StripPin = 5;        // MCP0 - LED strip (via MOSFET) 12V !!!
int nappe = 6;           // MCP0 - nappe Phréatique 12V !!!
int egg = 7;             // MCP0 - collecteur oeuf 12V !!!
int cave = 8;            // MCP0 - home 6
int toilette = 9;        // MCP0 - home 2
int sdb1 = 10;           // MCP0 - home 1
int arrosage = 11;       // MCP0 - home 4
int cuisine = 12;        // MCP0 - home 3
int lavelinge = 13;      // MCP0 - home 5



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
  mcp0.begin_I2C(0);                               // use default address 0 - GND/GND/GND
  for (int i = 0; i < 16 ; i++){
    mcp0.pinMode(i, OUTPUT);
    mcp0.digitalWrite(i, LOW);
    delay(50);
  }


  // MCP1, define all OUTPUT
  mcp1.begin_I2C(1);                               // use default address 1 - 5V/GND/GND
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
   stormPlayer.volume(26);                        // Set volume value (0~30).
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
// Pluie/orage > champ et jardin > nappe phréatique
// Lorsqu’il pleut, l’eau s’infiltre naturellement dans les sols et retourne progressivement à la nappe phréatique.

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS );    // wait 

// start scénario ---------------------------------------------------------------------------------------------------------
      
      mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
        piste4();                                             // piste audio pluie 24'

      mcp1.digitalWrite(pChamp, HIGH);                        // Valve ON pluie champs 
      mcp1.digitalWrite(pJardin, HIGH);                       // Valve ON pluie jardin
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );       // Wait
      mcp0.digitalWrite(nappe, HIGH);                         // LED ON nappe Phréatique
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );       // wait
      mcp1.digitalWrite(pChamp, LOW);                         // Valve OFF pluie champs
      mcp1.digitalWrite(pJardin, LOW);                        // Valve OFF pluie jardin
      mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );

      // on attend un peu pour l'histoire de la nappe Phréatique
      mcp0.digitalWrite(nappe, LOW);                          // LED OFF nappe Phréatique

// end scénario ---------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
      break;





    case S2:              /*---------------------- case Eau potable ---------------------*/
// captation d'eau potable dans la nappe Phréatique, utilisation en cuisine et évacuation à l'égout.

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait 

// start scénario -------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(nappe, HIGH);                           // LED ON nappe Phréatique
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );         // wait
      mcp0.digitalWrite(nappe, LOW);                            // LED OFF nappe
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );          // wait
      mcp0.digitalWrite(cuisine, HIGH);                         // LED ON led cuisine
      piste3();                                                 // piste audio robinet 20'
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );          // wait vaisselle, ambiance cuisine
      mcp1.digitalWrite(pump, HIGH);                            // Power ON pompe 
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

      mcp0.digitalWrite(cave, HIGH);                            // LED ON led Cave.
      mcp1.digitalWrite(sterput, HIGH);                         // Valve ON sterput, 
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );          // Wait trajet de l'eau jusqu'au collecteur rue
      mcp0.digitalWrite(egg, HIGH);                             // LED ON led Egg
      mcp1.digitalWrite(collecteur, HIGH);                      // Valve ON valve Egg  
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );         // wait end audio robinet
      mcp0.digitalWrite(cuisine, LOW);                          // LED OFF cuisine
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );          // wait
      mcp1.digitalWrite(sterput, LOW);                          // Valve OFF sertup
      mcp0.digitalWrite(cave, LOW);                             // LED OFF led cave
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );          // wait
      mcp1.digitalWrite(collecteur, LOW);                       // turn OFF the rest >
      mcp0.digitalWrite(egg, LOW);
      mcp1.digitalWrite(pump, LOW);

// end scénario ---------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;





    case S3:              /*---------------------- case Épuration ---------------------*/
// consommation d'eau ménagère, évacuation vers la station d'épuration et rivière

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS );                   // wait

// start scénario -------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(sdb1, HIGH);                                    // LED ON sdb
      piste5();                                                         // piste audio douche 24'
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );                  // wait
      mcp1.digitalWrite(pump, HIGH);                                    // Power ON pompe
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
      mcp1.digitalWrite(sterput, HIGH);                                 // Valve ON sterput
      mcp0.digitalWrite(cave, HIGH);                                    // LED ON cave
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );
      mcp0.digitalWrite(cuisine, HIGH);                                 // LED ON cuisine
      mcp0.digitalWrite(lavelinge, HIGH);                               // LED ON lavelinge
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 6 );                  // wait audio et simule l'eau arrivant au collecteur egg
      mcp0.digitalWrite(egg, HIGH);                                     // LED ON collecteur Egg
      mcp1.digitalWrite(collecteur, HIGH);                              // Valve ON collecteur Egg
      mcp0.digitalWrite(sdb1, LOW);                                     // LED OFF sdb
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );                  // wait
      mcp0.digitalWrite(lavelinge, LOW);                                // LED OFF lavelinge
      mcp0.digitalWrite(cuisine, LOW);                                  // LED OFF cuisine
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );                  // wait
      mcp1.digitalWrite(sterput, LOW);                                  // Valve OFF sterput
      mcp0.digitalWrite(cave, LOW);                                     // LED OFF cave
      mcp1.digitalWrite(bOrage, HIGH);                                  // Valve ON bassin d'orage
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );                  // wait, l'eau monte dans le champignon
      mcp1.digitalWrite(bOrage, LOW);                                   // Valve OFF bassin d'orage
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );                 // wait, l'eau transit jusqu'à la station d'épuration.
      mcp1.digitalWrite(riviere, HIGH);                                 // Valve ON rivière
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 15 );                 // wait
      mcp1.digitalWrite(collecteur, LOW);                               // Valve OFF collecteur Egg
      mcp0.digitalWrite(egg, LOW);                                      // LED OFF collecteur Egg
      mcp1.digitalWrite(riviere, LOW);                                  // Valve OFF rivière
      mcp1.digitalWrite(pump, LOW);                                     // Power OFF pompe

// end scénario ---------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;





    case S4:              /*---------------------- case Orage ---------------------*/
// Orage, la pluie coule de tous les côtés, débordement dans le bassin d'orage et vidange

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 1000 / portTICK_PERIOD_MS );                      // wait 

// start scénario -------------------------------------------------------------------------------------------------------

      storm(1);                                                       // thunder
      piste4();                                                       // piste audio pluie 24'
      mcp1.digitalWrite(pump, HIGH);                                  // Power ON pompe
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );
      mcp1.digitalWrite(tImper, HIGH);                                // Valve ON pluie toit G 
      mcp1.digitalWrite(tPlant, HIGH);                                // Valve ON pluie toit D 
      mcp1.digitalWrite(pJardin, HIGH);                               // Valve ON pluie jardin
      mcp1.digitalWrite(pChamp, HIGH);                                // Valve ON pluie champs 
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );               // wait, il pleut
      mcp1.digitalWrite(ruePlace, HIGH);                              // Valve ON rue&place
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );                // wait, l'eau coule sur la rue
      mcp1.digitalWrite(tImper, LOW);                                 // Valve OFF pluie toit G 
      mcp1.digitalWrite(tPlant, LOW);                                 // Valve OFF pluie toit D
      mcp1.digitalWrite(pJardin, LOW);                                // Valve OFF pluie jardin
      mcp1.digitalWrite(pChamp, LOW);                                 // Valve OFF pluie champs
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );                // wait, l'eau arrive dans le collecteur
    // l'orage est fini
      mcp0.digitalWrite(egg, HIGH);                                   // LED ON collecteur Egg
      mcp1.digitalWrite(collecteur, HIGH);                            // Valve ON collecteur Egg
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );                // wait, l'eau s'est écoulée des ruePlace
      mcp1.digitalWrite(ruePlace, LOW);                               // Valve OFF rue&place
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );               // wait, l'eau est dans le champignon, bOrage + led Haut + egout
      mcp1.digitalWrite(bOrage, HIGH);                                // Valve ON bassin d'orage
      mcp1.digitalWrite(ledBO1, HIGH);                                // LED ON bassin d'orage haut
      mcp1.digitalWrite(egout, HIGH);                                 // Valve ON egout
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 16 );               // wait, l'eau s'écoule dans le champignon
      mcp1.digitalWrite(collecteur, LOW);                             // Valve OFF collecteur Egg
      mcp0.digitalWrite(egg, LOW);                                    // LED OFF collecteur Egg
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );               // wait, 
    // le trop plein se déverse dans le bassin
      mcp1.digitalWrite(bOrage, LOW);                                 // Valve OFF bassin d'orage
      mcp1.digitalWrite(ledBO1, LOW);                                 // LED OFF bassin d'orage haut
      mcp1.digitalWrite(ledBO2, HIGH);                                // LED ON bassin d'orage bas
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );               // wait, vidange du BassinOrage
      mcp1.digitalWrite(ledBO2, LOW);                                 // LED OFF bassin d'orage bas
      mcp1.digitalWrite(egout, LOW);                                  // Valve OFF egout
        vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );                
      mcp1.digitalWrite(pump, LOW);                                   // Power OFF pompe

// end scénario ---------------------------------------------------------------------------------------------------------

      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;




    case S5:              /*---------------------- case Récup pluie ---------------------*/
// L'eau de pluie est récupérée en citerne et utilisée l'arrosage ou le lave linge

      suspendDemo();
      mcp0.digitalWrite(buttonID-1, HIGH);
        vTaskDelay( 500 / portTICK_PERIOD_MS );                   // wait

// start scénario -------------------------------------------------------------------------------------------------------

        piste4();                                                 // piste audio pluie 24'
        mcp1.digitalWrite(pump, HIGH);                            // Power ON pompe
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );          // wait, il pleut sur les toitures
        mcp1.digitalWrite(tImper, HIGH);                          // Valve ON pluie toit G 
        mcp1.digitalWrite(tPlant, HIGH);                          // Valve ON pluie toit D 
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );          // Wait, stockage de l'eau en citerne
        mcp1.digitalWrite(ledCiterne, HIGH);                      // LED ON citerne
        mcp1.digitalWrite(citerne, HIGH);                         // Valve ON citerne
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 13 );         // Wait, la citerne se remplit.
        mcp1.digitalWrite(tImper, LOW);                           // Valve OFF pluie toit G 
        mcp1.digitalWrite(tPlant, LOW);                           // Valve OFF pluie toit D 
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 5 );          // Wait
        mcp1.digitalWrite(citerne, LOW);                          // Valve OFF citerne
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 1 );

        piste1();                                                 // piste audio lavelinge 26'
        // mcp0.digitalWrite(lavelinge, HIGH);                      // LED ON lavelinge
        mcp0.digitalWrite(arrosage, HIGH);                        // LED ON arrosage
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 8 );          // wait lavelinge, ajout de la pompe d'arrosage.
        // mcp0.digitalWrite(lavelinge, LOW);                       // LED OFF lavelinge
        mcp0.digitalWrite(toilette, HIGH);                        // LED ON WC
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );          // wait

        piste2();                                                 // piste audio toilette 10'
        mcp0.digitalWrite(egg, HIGH);                             // LED ON collecteur Egg
        mcp1.digitalWrite(collecteur, HIGH);                      // Valve ON collecteur Egg
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );         // wait
        mcp0.digitalWrite(cave, HIGH);                            // LED ON cave
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 10 );         
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 3 );          // wait, la pompe d'arrosage est éteinte
        mcp0.digitalWrite(toilette, LOW);                         // LED OFF WC
        mcp0.digitalWrite(arrosage, LOW);                         // LED OFF arrosage
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * 2 );          // wait
        mcp1.digitalWrite(collecteur, LOW);                       // Valve OFF collecteur Egg
        mcp1.digitalWrite(pump, LOW);                             // Power OFF pompe
          vTaskDelay( (1000 / portTICK_PERIOD_MS) * .5 );         // wait
        mcp0.digitalWrite(cave, LOW);                             // LED OFF cave
        mcp0.digitalWrite(egg, LOW);                              // LED OFF collecteur Egg 
        mcp1.digitalWrite(ledCiterne, LOW);                       // LED OFF citerne

// end scénario ---------------------------------------------------------------------------------------------------------
      mcp0.digitalWrite(buttonID-1, LOW);
      etats = IDLE;
    break;
 
    }
    vTaskDelay(1);
  }
}
