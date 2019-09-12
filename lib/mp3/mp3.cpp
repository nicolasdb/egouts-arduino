#include <Arduino.h>
#include <mp3.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// mp3 variable
int StripPin = 9;    // LED strip (via MOSFET) connected to pin 9
int rxPin = 10;      // DFplayer RX to Arduino pin 10
int txPin = 11;      // DFplayer TX to Arduino pin 11
int busyPin = 12;    // DFplayer BUSY connected to pin 12

SoftwareSerial mySoftwareSerial(rxPin, txPin);
DFRobotDFPlayerMini stormPlayer;

void mp3setup () {
   pinMode(StripPin, OUTPUT);
   pinMode(busyPin, INPUT);
   pinMode(A0, INPUT);


   mySoftwareSerial.begin(9600);
   // Serial.begin(115200);
   Serial.println(F("Initializing DFPlayer..."));
   randomSeed(analogRead(A0));

   if (!stormPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
     Serial.println(F("Unable to begin. Check connection and SD card, or reset the Arduino."));
     while (true);
   }

   Serial.println(F("stormPlayer Mini online."));

   stormPlayer.setTimeOut(500);                   // Set serial communictaion time out 500ms
   stormPlayer.volume(25);                        // Set volume value (0~30).
   stormPlayer.EQ(DFPLAYER_EQ_NORMAL);              // Set EQ to BASS (normal/pop/rock/jazz/classic/bass)
   stormPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Set device we use SD as default
   // stormPlayer.enableDAC();                       // Enable On-chip DAC

}


void storm (int stormCount) {
    Serial.print("Boucles : ");
    Serial.println(stormCount);
for (size_t i=1 ; i<= stormCount ; i++){
    int flashCount = random (10, 15);        // Min. and max. number of flashes each loop
      int flashBrightnessMin =  10;           // LED flash min. brightness (0-255)
      int flashBrightnessMax =  255;          // LED flash max. brightness (0-255)

      int flashDurationMin = 1;               // Min. duration of each seperate flash
      int flashDurationMax = 50;              // Max. duration of each seperate flash

      int nextFlashDelayMin = 1;              // Min, delay between each flash and the next
      int nextFlashDelayMax = 150;            // Max, delay between each flash and the next

      int thunderDelay = random (300, 1000);  // Min. and max. delay between flashing and playing sound
      int thunderFile = random (1, 13);       // There are 17 soundfiles: 0001.mp3 ... 0017.mp3
      int loopDelay = random (50, 200);   // Min. and max. delay between each loop

      Serial.println();
      Serial.print(F("Flashing, count: "));
      Serial.println( flashCount );

      for (int flash = 0 ; flash <= flashCount; flash += 1) { // Flashing LED strip in a loop, random count

        analogWrite(StripPin, random (flashBrightnessMin, flashBrightnessMax)); // Turn LED strip on, random brightness
        delay(random(flashDurationMin, flashDurationMax)); // Keep it tured on, random duration

        analogWrite(StripPin, 0); // Turn the LED strip off
        delay(random(nextFlashDelayMin, nextFlashDelayMax)); // Random delay before next flash
      }

      Serial.print(F("Pausing before playing thunder sound, milliseconds: "));
      Serial.println(thunderDelay);
      delay(thunderDelay);

      Serial.print(F("Playing thunder sound, file number: "));
      Serial.println(thunderFile);
      stormPlayer.playFolder(2, thunderFile);
      delay(1000); // Give the DFPlayer some time

      while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
      }

      Serial.print(F("Pausing before next loop, milliseconds: "));
      Serial.println(loopDelay);
      delay(loopDelay);

  }
}

void piste1 () {
  stormPlayer.playFolder(1,1);        // lavelinge 26'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  // }
}

void piste2 () {
  stormPlayer.playFolder(1,2);        // wc 10'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  // }
}

void piste3 () {
  stormPlayer.playFolder(1,3);        // robinet 20'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  // }
}

void piste4 () {
  stormPlayer.playFolder(1,4);        // pluie 24'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
// }
}

void piste5 () {
  stormPlayer.playFolder(1,5);        // douche 24'
  delay(1000); // Give the DFPlayer some time

  // while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  // }
}
