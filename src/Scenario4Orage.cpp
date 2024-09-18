#include "Scenario4Orage.h"
#include "Arduino.h"
#include "pins.h"

extern Adafruit_MCP23X17 mcp0;
extern Adafruit_MCP23X17 mcp1;
extern MP3Player mp3Player;

void Scenario4Orage::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 4: Orage");
    }

    // start scénario
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
    mp3Player.piste3();                                     // piste audio orage 24'

    mcp1.digitalWrite(pChamp, HIGH);                        // Valve ON pluie champs 
    mcp1.digitalWrite(pJardin, HIGH);                       // Valve ON pluie jardin
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(egg, HIGH);                           // LED ON égout
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(cave, HIGH);                          // LED ON cave
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(ruePlace, HIGH);                      // LED ON rue et place
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(riviere, HIGH);                       // LED ON rivière
    delay(3000);                                            // wait 3 seconds

    mcp1.digitalWrite(pChamp, LOW);                         // Valve OFF pluie champs
    mcp1.digitalWrite(pJardin, LOW);                        // Valve OFF pluie jardin
    mcp0.digitalWrite(egg, LOW);                            // LED OFF égout
    mcp0.digitalWrite(cave, LOW);                           // LED OFF cave
    mcp1.digitalWrite(ruePlace, LOW);                       // LED OFF rue et place
    mcp1.digitalWrite(riviere, LOW);                        // LED OFF rivière
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 4: Orage completed");
    }
}