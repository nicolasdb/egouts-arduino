#ifndef SCENARIO4_ORAGE_H
#define SCENARIO4_ORAGE_H

#include "Scenario.h"

class Scenario4Orage : public Scenario {
public:
    Scenario4Orage(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : Scenario(mp3Player, mcp0, mcp1) {}
    void execute() override;

private:
    // Define pin numbers for this scenario
    static const int pump = 0;
    static const int tImper = 4;
    static const int tPlant = 5;
    static const int pJardin = 11;
    static const int pChamp = 10;
    static const int ruePlace = 13;
    static const int egg = 7;
    static const int collecteur = 12;
    static const int bOrage = 7;
    static const int ledBO1 = 9;
    static const int ledBO2 = 8;
    static const int egout = 6;
};

#endif // SCENARIO4_ORAGE_H