#ifndef CURSORMOUSE_H
#define CURSORMOUSE_H

#include "../Game/GameData.h"
#include "../Other/utils.h"
#include "../Other/Object.h"

enum CURSOR_TYPE
{
    DEFAULT_CURSOR,
    PISTOL_CURSOR,
    GOLDPISTOL_CURSOR,
    AK47_CURSOR,
    WIN94_CURSOR,
    SNIPER_CURSOR,

    CURSOR_COUNT
};

class CursorMouse;
CursorMouse *cursorMouse;

class CursorMouse : public Object
{
private:
    LTexture mCursor[CURSOR_COUNT];

    void loadIMG();

public:
    CursorMouse();

    void setCursor(CURSOR_TYPE cursor);
    //When shooting, the mouse will recoil upwards.
    void recoilMouse(int xRecoil, int yRecoil);
    void move(int valueX, int valueY);
    bool render() override;
};

CursorMouse::CursorMouse() : Object(false)
{
    loadIMG();
    setCursor(DEFAULT_CURSOR);

    // Lock mouse inside the window
    SDL_SetWindowGrab(gWindow, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void CursorMouse::loadIMG()
{
    //image: https://www.pngegg.com/en/,
    https://www.freepik.com/free-vector/funny-cursor-pack_1538099.htm#query=game%20cursor&position=6&from_view=keyword&track=ais&uuid=029b731f-41ca-4740-9cbf-0f380722a7a8
    mCursor[DEFAULT_CURSOR].loadFromFile(cursorMouseImagePath + "defaultcursor.png");
    mCursor[PISTOL_CURSOR].loadFromFile(cursorMouseImagePath + "pistol.png");
    mCursor[GOLDPISTOL_CURSOR].loadFromFile(cursorMouseImagePath + "goldpistol.png");
    mCursor[AK47_CURSOR].loadFromFile(cursorMouseImagePath + "ak47.png");
    mCursor[WIN94_CURSOR].loadFromFile(cursorMouseImagePath + "win94.png");
    mCursor[SNIPER_CURSOR].loadFromFile(cursorMouseImagePath + "sniper.png");
}

void CursorMouse::setCursor(CURSOR_TYPE cursor)
{
    mTexture = &mCursor[cursor];
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

    SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
}

bool CursorMouse::render()
{
    //get mouse pos
    SDL_GetMouseState(&mPosX, &mPosY);
    if (!checkCollision(mPosX, mPosY, 0, 0, 0, 0, SCREEN_WIDTH - getWidth(), SCREEN_HEIGHT - getHeight())) {
        setPosWhenInternalCollision(mPosX, mPosY, 0, 0, 0, 0, SCREEN_WIDTH - getWidth(), SCREEN_HEIGHT - getHeight());
        SDL_WarpMouseInWindow(gWindow, mPosX, mPosY);
    }

    mTexture->render(mPosX, mPosY);

    return true;
}

#endif