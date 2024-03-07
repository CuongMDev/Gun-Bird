#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 621;

int gVelocityYScene = 5;

const std::string imagePath = "image/";

SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

#endif