#ifndef SCENARIO5_RECUP_PLUIE_H
#define SCENARIO5_RECUP_PLUIE_H

#include "Scenario.h"

class Scenario5RecupPluie : public Scenario {
public:
    Scenario5RecupPluie(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : Scenario(mp3Player, mcp0, mcp1) {}
    void execute() override;

private:
    // Define pin numbers for this scenario
    static const int pump = 0;
    static const int tImper = 4;
    static const int tPlant = 5;
    static const int ledCiterne = 1;
    static const int citerne = 3;
    static const int arrosage = 11;
    static const int toilette = 9;
    static const int egg = 7;
    static const int collecteur = 12;
    static const int cave = 8;
};

#endif // SCENARIO5_RECUP_PLUIE_H