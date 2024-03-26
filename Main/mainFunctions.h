#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

#include "../objects/CursorMouse/CursorMouse.h"
#include <ctime>
#include <list>
#include "../objects/Other/Font.h"

bool init();

//Starts up SDL and creates window
bool initWindow();

bool loadIcon();

//Frees media and shuts down SDL
void close();

//-------------------------------------------

bool init()
{
    bool success = initWindow();
    loadFont();
    cursorMouse = new CursorMouse();

    //take random over time
    srand(time(0));

    return success;
}

bool initWindow()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    //Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    //Create window
    gWindow = SDL_CreateWindow("Gun-Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    if (!loadIcon()) {
        printf("SDL could not load icon! SDL Error: %s\n", SDL_GetError());
    }

    //Create vsynced renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    //Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

bool loadIcon()
{
    gIcon = loadSurfaceFromFile(iconImagePath + "/icon.png");
    if (gIcon == NULL) {
        return false;
    }

    SDL_SetWindowIcon(gWindow, gIcon);
    return true;
}

void close()
{
    // Clean up resources
    for (const auto& pair : firaFonts) {
        TTF_CloseFont(pair.second);
    }

    //Destroy window	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_FreeSurface(gIcon);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

#endif