#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

const int gInitVelocityYScene = 5;
const int speedChangeWhenLevelChange = 3;

const int waitTimeBeforePlaying = 2000;

Mix_Chunk* buttonClickSound;

//change when level change
int gCurVelocityYScene;
int gVelocityYScene;

int gMainBirdPosX;
int gMainBirdPosY;

bool gamePaused = false;

//-1:is stopping and can't start, 0: is stopping and can start, 1:is starting
int gameStarted = 0;

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
