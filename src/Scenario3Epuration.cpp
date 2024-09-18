#include "Scenario3Epuration.h"
#include "Arduino.h"
#include "pins.h"

extern Adafruit_MCP23X17 mcp0;
extern Adafruit_MCP23X17 mcp1;
extern MP3Player mp3Player;

void Scenario3Epuration::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 3: Epuration");
    }

    // start scénario
    mcp0.digitalWrite(sdb1, HIGH);                          // LED ON sdb
    mp3Player.piste5();                                     // piste audio douche 24'
    delay(5000);                                            // wait 5 seconds
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe
    delay(500);                                             // wait 0.5 seconds
    mcp1.digitalWrite(sterput, HIGH);                       // Valve ON sterput
    mcp0.digitalWrite(cave, HIGH);                          // LED ON cave
    delay(1000);                                            // wait 1 second
    mcp0.digitalWrite(cuisine, HIGH);                       // LED ON cuisine
    mcp0.digitalWrite(lavelinge, HIGH);                     // LED ON lavelinge
    delay(6000);                                            // wait 6 seconds
    mcp0.digitalWrite(egg, HIGH);                           // LED ON collecteur Egg
    mcp1.digitalWrite(collecteur, HIGH);                    // Valve ON collecteur Egg
    mcp0.digitalWrite(sdb1, LOW);                           // LED OFF sdb
    delay(1000);                                            // wait 1 second
    mcp0.digitalWrite(lavelinge, LOW);                      // LED OFF lavelinge
    mcp0.digitalWrite(cuisine, LOW);                        // LED OFF cuisine
    delay(2000);                                            // wait 2 seconds
    mcp1.digitalWrite(sterput, LOW);                        // Valve OFF sterput
    mcp0.digitalWrite(cave, LOW);                           // LED OFF cave
    mcp1.digitalWrite(bOrage, HIGH);                        // Valve ON bassin d'orage
    delay(2000);                                            // wait 2 seconds
    mcp1.digitalWrite(bOrage, LOW);                         // Valve OFF bassin d'orage
    delay(15000);                                           // wait 15 seconds
    mcp1.digitalWrite(riviere, HIGH);                       // Valve ON rivière
    delay(15000);                                           // wait 15 seconds
    mcp1.digitalWrite(collecteur, LOW);                     // Valve OFF collecteur Egg
    mcp0.digitalWrite(egg, LOW);                            // LED OFF collecteur Egg
    mcp1.digitalWrite(riviere, LOW);                        // Valve OFF rivière
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 3: Epuration completed");
    }
}