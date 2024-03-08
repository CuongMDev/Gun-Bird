#ifndef AIMMOUSE_H
#define AIMMOUSE_H

#include "mainData.h
#include "utils.h"

void loadCursorMouseIMG()
{
    //image: https://www.pngegg.com/en/
    const char *path = (imagePath + "aim0.png").c_str();
    SDL_Surface *cursorSurface = IMG_Load(path);
    if (cursorSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return;
    }

    SDL_SetColorKey(cursorSurface, SDL_TRUE, SDL_MapRGB(cursorSurface->format, 255, 255, 255));
    SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    if (cursor == NULL) {
        printf("Failed to create custom cursor! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_SetCursor(cursor);
    SDL_FreeSurface(cursorSurface);
}

//"When shooting, the mouse will recoil upwards."
void recoilMouse(int xRecoil, int yRecoil)
{
    xRecoil = getRandomNumber(std::max(-xRecoil, xRecoil), xRecoil);
    yRecoil = getRandomNumber(std::max(0, yRecoil), yRecoil);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_WarpMouseInWindow(gWindow, mouseX + xRecoil, std::max(0, mouseY - yRecoil));
}

#endif