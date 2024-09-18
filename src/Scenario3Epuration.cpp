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
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
    mp3Player.piste2();                                     // piste audio épuration 24'

    mcp0.digitalWrite(egg, HIGH);                           // LED ON égout
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(toilette, HIGH);                      // LED ON toilette
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(sdb1, HIGH);                          // LED ON salle de bain 1
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(cuisine, HIGH);                       // LED ON cuisine
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(lavelinge, HIGH);                     // LED ON lave-linge
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(riviere, HIGH);                       // LED ON rivière
    delay(3000);                                            // wait 3 seconds

    mcp0.digitalWrite(egg, LOW);                            // LED OFF égout
    mcp0.digitalWrite(toilette, LOW);                       // LED OFF toilette
    mcp0.digitalWrite(sdb1, LOW);                           // LED OFF salle de bain 1
    mcp0.digitalWrite(cuisine, LOW);                        // LED OFF cuisine
    mcp0.digitalWrite(lavelinge, LOW);                      // LED OFF lave-linge
    mcp1.digitalWrite(riviere, LOW);                        // LED OFF rivière
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 3: Epuration completed");
    }
}