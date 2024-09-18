#include "Scenario5RecupPluie.h"
#include "Arduino.h"

void Scenario5RecupPluie::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 5: Récup Pluie");
    }

    // L'eau de pluie est récupérée en citerne et utilisée pour l'arrosage ou le lave-linge
    mp3Player.piste4();                                               // piste audio pluie 24'
    mcp1.digitalWrite(pump, HIGH);                                    // Power ON pompe
    delay(1000);                                                      // wait 1 second, il pleut sur les toitures
    mcp1.digitalWrite(tImper, HIGH);                                  // Valve ON pluie toit G 
    mcp1.digitalWrite(tPlant, HIGH);                                  // Valve ON pluie toit D 
    delay(5000);                                                      // Wait 5 seconds, stockage de l'eau en citerne
    mcp1.digitalWrite(ledCiterne, HIGH);                              // LED ON citerne
    mcp1.digitalWrite(citerne, HIGH);                                 // Valve ON citerne
    delay(13000);                                                     // Wait 13 seconds, la citerne se remplit.
    mcp1.digitalWrite(tImper, LOW);                                   // Valve OFF pluie toit G 
    mcp1.digitalWrite(tPlant, LOW);                                   // Valve OFF pluie toit D 
    delay(5000);                                                      // Wait 5 seconds
    mcp1.digitalWrite(citerne, LOW);                                  // Valve OFF citerne
    delay(1000);                                                      // wait 1 second

    mp3Player.piste1();                                               // piste audio lavelinge 26'
    mcp0.digitalWrite(arrosage, HIGH);                                // LED ON arrosage
    delay(8000);                                                      // wait 8 seconds lavelinge, ajout de la pompe d'arrosage.
    mcp0.digitalWrite(toilette, HIGH);                                // LED ON WC
    delay(3000);                                                      // wait 3 seconds

    mp3Player.piste2();                                               // piste audio toilette 10'
    mcp0.digitalWrite(egg, HIGH);                                     // LED ON collecteur Egg
    mcp1.digitalWrite(collecteur, HIGH);                              // Valve ON collecteur Egg
    delay(500);                                                       // wait 0.5 seconds
    mcp0.digitalWrite(cave, HIGH);                                    // LED ON cave
    delay(13000);                                                     // wait 13 seconds
    mcp0.digitalWrite(toilette, LOW);                                 // LED OFF WC
    mcp0.digitalWrite(arrosage, LOW);                                 // LED OFF arrosage
    delay(2000);                                                      // wait 2 seconds
    mcp1.digitalWrite(collecteur, LOW);                               // Valve OFF collecteur Egg
    mcp1.digitalWrite(pump, LOW);                                     // Power OFF pompe
    delay(500);                                                       // wait 0.5 seconds
    mcp0.digitalWrite(cave, LOW);                                     // LED OFF cave
    mcp0.digitalWrite(egg, LOW);                                      // LED OFF collecteur Egg 
    mcp1.digitalWrite(ledCiterne, LOW);                               // LED OFF citerne

    if (DEBUG_MODE) {
        Serial.println("Scenario 5: Récup Pluie completed");
    }
}