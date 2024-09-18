#include "Scenario2EauPotable.h"
#include "Arduino.h"

void Scenario2EauPotable::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 2: Eau Potable");
    }

    // captation d'eau potable dans la nappe Phréatique, utilisation en cuisine et évacuation à l'égout.
    mcp0.digitalWrite(nappe, HIGH);                           // LED ON nappe Phréatique
    delay(10000);                                             // wait 10 seconds
    mcp0.digitalWrite(nappe, LOW);                            // LED OFF nappe
    delay(1000);                                              // wait 1 second
    mcp0.digitalWrite(cuisine, HIGH);                         // LED ON led cuisine
    mp3Player.piste3();                                       // piste audio robinet 20'
    delay(2000);                                              // wait vaisselle, ambiance cuisine
    mcp1.digitalWrite(pump, HIGH);                            // Power ON pompe 
    delay(1000);                                              // wait 1 second

    mcp0.digitalWrite(cave, HIGH);                            // LED ON led Cave.
    mcp1.digitalWrite(sterput, HIGH);                         // Valve ON sterput, 
    delay(3000);                                              // Wait trajet de l'eau jusqu'au collecteur rue
    mcp0.digitalWrite(egg, HIGH);                             // LED ON led Egg
    mcp1.digitalWrite(collecteur, HIGH);                      // Valve ON valve Egg  
    delay(10000);                                             // wait end audio robinet
    mcp0.digitalWrite(cuisine, LOW);                          // LED OFF cuisine
    delay(2000);                                              // wait 2 seconds
    mcp1.digitalWrite(sterput, LOW);                          // Valve OFF sertup
    mcp0.digitalWrite(cave, LOW);                             // LED OFF led cave
    delay(2000);                                              // wait 2 seconds
    mcp1.digitalWrite(collecteur, LOW);                       // turn OFF the rest
    mcp0.digitalWrite(egg, LOW);
    mcp1.digitalWrite(pump, LOW);

    if (DEBUG_MODE) {
        Serial.println("Scenario 2: Eau Potable completed");
    }
}