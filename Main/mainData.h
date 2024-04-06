#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 630;

//path---
const std::string imagePath = "image/";
    const std::string bulletImagePath = imagePath + "Bullet/";
    const std::string characterImagePath = imagePath + "Character/";
        const std::string mainBirdImagePath = characterImagePath + "MainBird/";
        const std::string batImagePath = characterImagePath + "Bat/";
        const std::string bossImagePath = characterImagePath + "Boss/";
    const std::string cursorMouseImagePath = imagePath + "CursorMouse/";
    const std::string gameOverImagePath = imagePath + "GameOver/";
    const std::string gunImagePath = imagePath + "Gun/";
    const std::string healthBarImagePath = imagePath + "HealthBar/";
    const std::string iconImagePath = imagePath + "Icon/";
    const std::string itemImagePath = imagePath + "Item/";
    const std::string sceneImagePath = imagePath + "Scene/";

const std::string soundPath = "sound/";
    const std::string gunSoundPath = soundPath + "Gun/";
    const std::string characterSoundPath = soundPath + "Character/";
        const std::string mainBirdSoundPath = characterSoundPath + "MainBird/";
        const std::string bossSoundPath = characterSoundPath + "Boss/";

const std::string fontPath = "font/";
    const std::string firaCodeFontPath = fontPath + "FiraCode/";
//---

SDL_Window *gWindow = NULL;
//The window renderer
SDL_Renderer *gRenderer = NULL;

SDL_Surface *gIcon = NULL;

#endif