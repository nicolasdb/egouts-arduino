#ifndef SCENARIO2_EAU_POTABLE_H
#define SCENARIO2_EAU_POTABLE_H

#include "Scenario.h"

class Scenario2EauPotable : public Scenario {
public:
    Scenario2EauPotable(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : Scenario(mp3Player, mcp0, mcp1) {}
    void execute() override;

private:
    // Define pin numbers for this scenario
    static const int pump = 0;
    static const int nappe = 6;
    static const int cuisine = 12;
    static const int cave = 8;
    static const int sterput = 2;
    static const int egg = 7;
    static const int collecteur = 12;
};

#endif // SCENARIO2_EAU_POTABLE_H