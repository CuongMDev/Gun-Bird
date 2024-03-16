#ifndef CURSORMOUSE_H
#define CURSORMOUSE_H

#include "../../Main/mainData.h"
#include "../Other/utils.h"

//Load image surface at specified path
SDL_Surface *loadSurfaceFromFile(const std::string &path, const bool &removeBKG = false, const Uint8 &r = 0, const Uint8 &g = 0, const Uint8 &b = 0);

enum CURSOR_TYPE
{
    DEFAULT_CURSOR,
    AIM_CURSOR,

    CURSOR_COUNT
};

class CursorMouse
{
private:
    static SDL_Cursor *mCursor[CURSOR_COUNT];

    static SDL_Cursor *loadCursorFromFile(const std::string &path);

    static void loadIMG();
public:
    CursorMouse();
    ~CursorMouse();

    static void setCursor(CURSOR_TYPE cursor);
    //When shooting, the mouse will recoil upwards.
    static void recoilMouse(int xRecoil, int yRecoil);
    static void move(int valueX, int valueY);
};

SDL_Cursor* CursorMouse::mCursor[CURSOR_COUNT] = {NULL};

CursorMouse::CursorMouse()
{
    loadIMG();
    setCursor(DEFAULT_CURSOR);
}

CursorMouse::~CursorMouse()
{
    for (int i = 0; i < CURSOR_COUNT; i++) {
        SDL_FreeCursor(mCursor[i]);
    }
}

SDL_Cursor *CursorMouse::loadCursorFromFile(const std::string &path)
{
    SDL_Surface *cursorSurface = loadSurfaceFromFile(path);
    if (cursorSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return NULL;
    }

    SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    if (cursor == NULL) {
        printf("Failed to create custom cursor! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(cursorSurface);
    return cursor;
}

void CursorMouse::loadIMG()
{
    //image: https://www.pngegg.com/en/,
    https://www.freepik.com/free-vector/funny-cursor-pack_1538099.htm#query=game%20cursor&position=6&from_view=keyword&track=ais&uuid=029b731f-41ca-4740-9cbf-0f380722a7a8
    mCursor[DEFAULT_CURSOR] = loadCursorFromFile(cursorMouseImagePath + "defaultcursor.png");
    mCursor[AIM_CURSOR] = loadCursorFromFile(cursorMouseImagePath + "aim0.png");
}

void CursorMouse::setCursor(CURSOR_TYPE cursor)
{
    SDL_SetCursor(mCursor[cursor]);
}

//"When shooting, the mouse will recoil upwards."
void CursorMouse::recoilMouse(int xRecoil, int yRecoil)
{
    xRecoil = getRandomNumber(std::max(-xRecoil, xRecoil), xRecoil);
    yRecoil = getRandomNumber(std::max(0, yRecoil), yRecoil);

    move(xRecoil, -yRecoil);
}

void CursorMouse::move(int valueX, int valueY)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    mouseX += valueX, mouseY += valueY;
    if (!checkCollision(mouseX, mouseY, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        setPosToBorderPos(mouseX, mouseY, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
}

SDL_Surface *loadSurfaceFromFile(const std::string &path, const bool &removeBKG, const Uint8 &r, const Uint8 &g, const Uint8 &b) {
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return NULL;
    }
    else {
        //Color key image
        if (removeBKG) {
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, r, g, b));
        }

        return loadedSurface;
    }
}

#endif