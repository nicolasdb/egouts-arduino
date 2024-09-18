#include "MP3Player.h"
#include <Arduino.h>

MP3Player::MP3Player() : mySoftwareSerial(RX_PIN, TX_PIN) {}

void MP3Player::begin() {
    mySoftwareSerial.begin(9600);
    
    if (DEBUG_MODE) {
        Serial.println(F("Initializing DFPlayer..."));
    }

    if (!player.begin(mySoftwareSerial)) {
        if (DEBUG_MODE) {
            Serial.println(F("Unable to begin DFPlayer. Check connection and SD card."));
        }
        while(true);
    }

    if (DEBUG_MODE) {
        Serial.println(F("DFPlayer Mini online."));
    }

    player.setTimeOut(500);
    player.volume(26);
    player.EQ(DFPLAYER_EQ_NORMAL);
    player.outputDevice(DFPLAYER_DEVICE_SD);
}

void MP3Player::playTrack(int folder, int track) {
    player.playFolder(folder, track);
    delay(1000); // Give the DFPlayer some time
}

void MP3Player::storm(uint32_t stormCount) {
    if (DEBUG_MODE) {
        Serial.print("Boucles : ");
        Serial.println(stormCount);
    }

    for (size_t i = 1; i <= stormCount; i++) {
        uint32_t flashCount = random(10, 15);
        int flashDurationMin = 1;
        int flashDurationMax = 50;
        int nextFlashDelayMin = 1;
        int nextFlashDelayMax = 150;
        int thunderDelay = random(300, 1000);
        int thunderFile = random(1, 7);
        int loopDelay = random(50, 200);

        if (DEBUG_MODE) {
            Serial.println();
            Serial.print(F("Flashing, count: "));
            Serial.println(flashCount);
        }

        // Flashing LED strip logic should be moved to a separate LED control class
        // For now, we'll just simulate the delays

        for (uint32_t flash = 0; flash <= flashCount; flash += 1) {
            delay(random(flashDurationMin, flashDurationMax));
            delay(random(nextFlashDelayMin, nextFlashDelayMax));
        }

        if (DEBUG_MODE) {
            Serial.print(F("Pausing before playing thunder sound, milliseconds: "));
            Serial.println(thunderDelay);
        }
        delay(thunderDelay);

        if (DEBUG_MODE) {
            Serial.print(F("Playing thunder sound, file number: "));
            Serial.println(thunderFile);
        }
        playTrack(1, thunderFile);

        while (digitalRead(BUSY_PIN) == LOW) {
            // Wait for the DFPlayer to finish playing the MP3 file
        }

        if (DEBUG_MODE) {
            Serial.print(F("Pausing before next loop, milliseconds: "));
            Serial.println(loopDelay);
        }
        delay(loopDelay);
    }
}

void MP3Player::piste1() {
    playTrack(2, 1);  // lavelinge 26'
}

void MP3Player::piste2() {
    playTrack(2, 2);  // wc 10'
}

void MP3Player::piste3() {
    playTrack(2, 3);  // robinet 20'
}

void MP3Player::piste4() {
    playTrack(2, 4);  // pluie 24'
}

void MP3Player::piste5() {
    playTrack(2, 5);  // douche 24'
}