#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SDL2/SDL.h>

const int gInitVelocityYScene = 5;
const int speedChangeWhenLevelChange = 3;

const int waitTimeBeforePlaying = 2000;

//change when level change
int gCurVelocityYScene;
int gVelocityYScene;

int gMainBirdPosX;
int gMainBirdPosY;

bool gamePaused = false;

//-1 if game not pause
int startPauseTime = -1;

int pausedTime = 0;

int getCurrentTime();

int getCurrentTime()
{
    if (gamePaused) {
        return startPauseTime - pausedTime;
    }
    return SDL_GetTicks() - pausedTime;
}

#endif //GAMEDATA_H
