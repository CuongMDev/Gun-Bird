#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 630;

const int gInitVelocityYScene = 5;

const int waitTimeBeforePlaying = 2000;

//pipe will be created when current time = this
int gVelocityYScene = gInitVelocityYScene;


//path---
const std::string imagePath = "image/";
    const std::string bulletImagePath = imagePath + "Bullet/";
    const std::string characterImagePath = imagePath + "Character/";
        const std::string mainBirdImagePath = characterImagePath + "MainBird/";
        const std::string batImagePath = characterImagePath + "Bat/";
    const std::string cursorMouseImagePath = imagePath + "CursorMouse/";
    const std::string gameOverImagePath = imagePath + "GameOver/";
    const std::string gunImagePath = imagePath + "Gun/";
    const std::string healthBarImagePath = imagePath + "HealthBar/";
    const std::string iconImagePath = imagePath + "Icon/";
    const std::string sceneImagePath = imagePath + "Scene/";

const std::string fontPath = "font/";
    const std::string firaCodeFontPath = fontPath + "FiraCode/";
//---

SDL_Window *gWindow = NULL;
//The window renderer
SDL_Renderer *gRenderer = NULL;

SDL_Surface *gIcon = NULL;

#endif