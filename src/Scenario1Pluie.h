#ifndef SCENARIO1_PLUIE_H
#define SCENARIO1_PLUIE_H

#include "Scenario.h"

class Scenario1Pluie : public Scenario {
public:
    Scenario1Pluie(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : Scenario(mp3Player, mcp0, mcp1) {}
    void execute() override;

private:
    // Define pin numbers for this scenario
    static const int pump = 0;
    static const int pChamp = 10;
    static const int pJardin = 11;
    static const int nappe = 6;
};

#endif // SCENARIO1_PLUIE_H