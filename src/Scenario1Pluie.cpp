#include "Scenario1Pluie.h"
#include "Arduino.h"
#include "pins.h"

extern Adafruit_MCP23X17 mcp0;
extern Adafruit_MCP23X17 mcp1;
extern MP3Player mp3Player;

void Scenario1Pluie::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 1: Pluie");
    }

    // start scénario
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
    mp3Player.piste4();                                     // piste audio pluie 24'

    mcp1.digitalWrite(pChamp, HIGH);                        // Valve ON pluie champs 
    mcp1.digitalWrite(pJardin, HIGH);                       // Valve ON pluie jardin
    delay(15000);                                           // Wait 15 seconds
    mcp0.digitalWrite(nappe, HIGH);                         // LED ON nappe Phréatique
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(pChamp, LOW);                         // Valve OFF pluie champs
    mcp1.digitalWrite(pJardin, LOW);                        // Valve OFF pluie jardin
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe
    delay(3000);                                            // wait 3 seconds

    // on attend un peu pour l'histoire de la nappe Phréatique
    mcp0.digitalWrite(nappe, LOW);                          // LED OFF nappe Phréatique

    if (DEBUG_MODE) {
        Serial.println("Scenario 1: Pluie completed");
    }
}