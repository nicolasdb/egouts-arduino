#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "config.h"

class MP3Player {
public:
    MP3Player();
    void begin();
    void playTrack(int folder, int track);
    void storm(uint32_t stormCount);
    void piste1();
    void piste2();
    void piste3();
    void piste4();
    void piste5();

private:
    SoftwareSerial mySoftwareSerial;
    DFRobotDFPlayerMini player;
};

#endif // MP3PLAYER_H