#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

#include "mainData.h"
#include "CursorMouse.h"
#include "objects.h"
#include <ctime>
#include <list>

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

void renderPipe(std::list<Pipe*>& pipeList);

void startPlaying();

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    loadCursorMouseIMG();
    //take random over time
    srand(time(0));

    return success;
}

void close()
{
    //Destroy window	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void renderPipe(std::list<Pipe*>& pipeList)
{
    if (SDL_GetTicks() >= createdTime) {
        //random flip
        bool flip = getRandomNumber(0, 1);
        //pipe height
        int pipeHeight = getRandomNumber(50, 300);
        //create new pipe
        Pipe* pipe = new Pipe(getRandomNumber(50, SCREEN_HEIGHT / 2), flip);
        pipeList.push_back(pipe);

        //reset created time
        createdTime = SDL_GetTicks() + getRandomNumber(500, 1000);
    }

    //render pipe
    for (auto pipe = pipeList.begin(); pipe != pipeList.end();) {
        if (!(*pipe)->render()) {
            delete (*pipe);
            pipe = pipeList.erase(pipe);
        }
        else pipe++;
    }
}

void startPlaying()
{
}

#endif