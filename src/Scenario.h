#ifndef SCENARIO_H
#define SCENARIO_H

#include "config.h"
#include "MP3Player.h"
#include <Adafruit_MCP23X17.h>

class Scenario {
public:
    Scenario(MP3Player& mp3Player, Adafruit_MCP23X17& mcp0, Adafruit_MCP23X17& mcp1)
        : mp3Player(mp3Player), mcp0(mcp0), mcp1(mcp1) {}
    virtual void execute() = 0;

protected:
    MP3Player& mp3Player;
    Adafruit_MCP23X17& mcp0;
    Adafruit_MCP23X17& mcp1;
};

#endif // SCENARIO_H