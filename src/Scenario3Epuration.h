#ifndef SCENARIO3_EPURATION_H
#define SCENARIO3_EPURATION_H

#include "Scenario.h"

class Scenario3Epuration : public Scenario {
public:
    Scenario3Epuration(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : Scenario(mp3Player, mcp0, mcp1) {}
    void execute() override;

private:
    // Define pin numbers for this scenario
    static const int pump = 0;
    static const int sdb1 = 10;
    static const int sterput = 2;
    static const int cave = 8;
    static const int cuisine = 12;
    static const int lavelinge = 13;
    static const int egg = 7;
    static const int collecteur = 12;
    static const int bOrage = 7;
    static const int riviere = 14;
};

#endif // SCENARIO3_EPURATION_H