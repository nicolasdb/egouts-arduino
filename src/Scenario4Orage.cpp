#include "Scenario4Orage.h"
#include "Arduino.h"

void Scenario4Orage::execute() {
    if (DEBUG_MODE) {
        Serial.println("Executing Scenario 4: Orage");
    }

    // Orage, la pluie coule de tous les côtés, débordement dans le bassin d'orage et vidange
    mp3Player.storm(1);                                               // thunder
    mp3Player.piste4();                                               // piste audio pluie 24'
    mcp1.digitalWrite(pump, HIGH);                                    // Power ON pompe
    delay(500);                                                       // wait 0.5 seconds
    mcp1.digitalWrite(tImper, HIGH);                                  // Valve ON pluie toit G 
    mcp1.digitalWrite(tPlant, HIGH);                                  // Valve ON pluie toit D 
    mcp1.digitalWrite(pJardin, HIGH);                                 // Valve ON pluie jardin
    mcp1.digitalWrite(pChamp, HIGH);                                  // Valve ON pluie champs 
    delay(10000);                                                     // wait 10 seconds, il pleut
    mcp1.digitalWrite(ruePlace, HIGH);                                // Valve ON rue&place
    delay(5000);                                                      // wait 5 seconds, l'eau coule sur la rue
    mcp1.digitalWrite(tImper, LOW);                                   // Valve OFF pluie toit G 
    mcp1.digitalWrite(tPlant, LOW);                                   // Valve OFF pluie toit D
    mcp1.digitalWrite(pJardin, LOW);                                  // Valve OFF pluie jardin
    mcp1.digitalWrite(pChamp, LOW);                                   // Valve OFF pluie champs
    delay(1000);                                                      // wait 1 second, l'eau arrive dans le collecteur
    // l'orage est fini
    mcp0.digitalWrite(egg, HIGH);                                     // LED ON collecteur Egg
    mcp1.digitalWrite(collecteur, HIGH);                              // Valve ON collecteur Egg
    delay(3000);                                                      // wait 3 seconds, l'eau s'est écoulée des ruePlace
    mcp1.digitalWrite(ruePlace, LOW);                                 // Valve OFF rue&place
    delay(500);                                                       // wait 0.5 seconds, l'eau est dans le champignon, bOrage + led Haut + egout
    mcp1.digitalWrite(bOrage, HIGH);                                  // Valve ON bassin d'orage
    mcp1.digitalWrite(ledBO1, HIGH);                                  // LED ON bassin d'orage haut
    mcp1.digitalWrite(egout, HIGH);                                   // Valve ON egout
    delay(16000);                                                     // wait 16 seconds, l'eau s'écoule dans le champignon
    mcp1.digitalWrite(collecteur, LOW);                               // Valve OFF collecteur Egg
    mcp0.digitalWrite(egg, LOW);                                      // LED OFF collecteur Egg
    delay(10000);                                                     // wait 10 seconds
    // le trop plein se déverse dans le bassin
    mcp1.digitalWrite(bOrage, LOW);                                   // Valve OFF bassin d'orage
    mcp1.digitalWrite(ledBO1, LOW);                                   // LED OFF bassin d'orage haut
    mcp1.digitalWrite(ledBO2, HIGH);                                  // LED ON bassin d'orage bas
    delay(10000);                                                     // wait 10 seconds, vidange du BassinOrage
    mcp1.digitalWrite(ledBO2, LOW);                                   // LED OFF bassin d'orage bas
    mcp1.digitalWrite(egout, LOW);                                    // Valve OFF egout
    delay(1000);                                                      // wait 1 second
    mcp1.digitalWrite(pump, LOW);                                     // Power OFF pompe

    if (DEBUG_MODE) {
        Serial.println("Scenario 4: Orage completed");
    }
}