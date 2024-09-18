#include "Scenario5RecupPluie.h"
#include "Arduino.h"
#include "pins.h"

extern Adafruit_MCP23X17 mcp0;
extern Adafruit_MCP23X17 mcp1;
extern MP3Player mp3Player;

void Scenario5RecupPluie::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 5: Récupération Pluie");
    }

    // start scénario
    mcp1.digitalWrite(pump, HIGH);                          // Power ON pompe 
    mp3Player.piste5();                                     // piste audio récupération pluie 24'

    mcp1.digitalWrite(pChamp, HIGH);                        // Valve ON pluie champs 
    mcp1.digitalWrite(pJardin, HIGH);                       // Valve ON pluie jardin
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(ledCiterne, HIGH);                    // LED ON citerne
    mcp1.digitalWrite(citerne, HIGH);                       // Valve ON citerne
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(tImper, HIGH);                        // LED ON toiture imperméable
    delay(3000);                                            // wait 3 seconds
    mcp1.digitalWrite(tPlant, HIGH);                        // LED ON toiture plantée
    delay(3000);                                            // wait 3 seconds
    mcp0.digitalWrite(arrosage, HIGH);                      // LED ON arrosage
    delay(3000);                                            // wait 3 seconds

    mcp1.digitalWrite(pChamp, LOW);                         // Valve OFF pluie champs
    mcp1.digitalWrite(pJardin, LOW);                        // Valve OFF pluie jardin
    mcp1.digitalWrite(ledCiterne, LOW);                     // LED OFF citerne
    mcp1.digitalWrite(citerne, LOW);                        // Valve OFF citerne
    mcp1.digitalWrite(tImper, LOW);                         // LED OFF toiture imperméable
    mcp1.digitalWrite(tPlant, LOW);                         // LED OFF toiture plantée
    mcp0.digitalWrite(arrosage, LOW);                       // LED OFF arrosage
    mcp1.digitalWrite(pump, LOW);                           // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 5: Récupération Pluie completed");
    }
}