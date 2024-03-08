#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 630;

//pipe will be created when current time = this
Uint32 createdTime = 2000;
int gVelocityYScene = 5;
bool gameOver = false;

const std::string imagePath = "image/";

SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

#endif