#include "Scenario2EauPotable.h"
#include "Arduino.h"
#include "pins.h"

extern Adafruit_MCP23X17 mcp0;
extern Adafruit_MCP23X17 mcp1;
extern MP3Player mp3Player;

void Scenario2EauPotable::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 2: Eau Potable");
    }

    // start scénario
    mcp0.digitalWrite(nappe, HIGH);                         // LED ON nappe Phréatique
    delay(10000);                                           // wait 10 seconds
    mcp0.digitalWrite(nappe, LOW);                          // LED OFF nappe
    delay(1000);                                            // wait 1 second
    mcp0.digitalWrite(cuisine, HIGH);                       // LED ON cuisine
    mp3Player.piste3();                                     // piste audio robinet 20'
    delay(2000);                                            // wait 2 seconds
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
    delay(1000);                                            // wait 1 second

    mcp0.digitalWrite(cave, HIGH);                          // LED ON cave
    mcp1.digitalWrite(sterput, HIGH);                       // Valve ON sterput
    delay(3000);                                            // Wait 3 seconds
    mcp0.digitalWrite(egg, HIGH);                           // LED ON egg
    mcp1.digitalWrite(collecteur, HIGH);                    // Valve ON valve Egg  
    delay(10000);                                           // wait 10 seconds
    mcp0.digitalWrite(cuisine, LOW);                        // LED OFF cuisine
    delay(2000);                                            // wait 2 seconds
    mcp1.digitalWrite(sterput, LOW);                        // Valve OFF sterput
    mcp0.digitalWrite(cave, LOW);                           // LED OFF cave
    delay(2000);                                            // wait 2 seconds
    mcp1.digitalWrite(collecteur, LOW);                     // Valve OFF collecteur
    mcp0.digitalWrite(egg, LOW);                            // LED OFF egg
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 2: Eau Potable completed");
    }
}