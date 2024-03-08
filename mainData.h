#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 630;
const int gInitVelocityYScene = 5;

//pipe will be created when current time = this
Uint32 createdTime = 2000;
int gVelocityYScene = gInitVelocityYScene;

bool gameOver = false;

const std::string imagePath = "image/";
const std::string gunImagePath = imagePath + "gun/";

SDL_Window *gWindow = NULL;
//The window renderer
SDL_Renderer *gRenderer = NULL;

#endif