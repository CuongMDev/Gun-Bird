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

    //use to save pos & type
    //-1 if it has not saved
    int sMouseX, sMouseY;
    CURSOR_TYPE sCursor;

    CURSOR_TYPE currentCursor;

    void loadIMG();

public:
    CursorMouse();

    //change x1, y1 to velocityX1, velocityY1
    void calculateVelocityToMouse(int &x, int &y, const double &speed);
    double angleToMousePos(const int &x, const int &y);

    void setCursor(CURSOR_TYPE cursor);
    //When shooting, the mouse will recoil upwards.
    void recoilMouse(int xRecoil, int yRecoil);
    void move(int valueX, int valueY);
    bool render() override;

    void getAimPos(int &x, int &y);

    void saveCursor();
    void loadSavedCursor();
};

CursorMouse::CursorMouse() : Object(false)
{
    loadIMG();
    setCursor(DEFAULT_CURSOR);

    // Lock mouse inside the window
    SDL_SetWindowGrab(gWindow, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    currentCursor = DEFAULT_CURSOR;

    sMouseX = -1, sMouseY = -1;
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

double CursorMouse::angleToMousePos(const int &x, const int &y)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseX += getWidth() / 4;
    mouseY += getHeight() / 4;

    return angleBetweenTwoPos(x, y, mouseX, mouseY);
}

void CursorMouse::calculateVelocityToMouse(int &x, int &y, const double &speed)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseX += getWidth() / 4;
    mouseY += getHeight() / 4;

    calculateVelocityBetweenTwoPos(x, y, mouseX, mouseY, speed);
}

void CursorMouse::setCursor(CURSOR_TYPE cursor)
{
    currentCursor = cursor;
    mTexture = &mCursor[currentCursor];
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

void CursorMouse::saveCursor()
{
    SDL_GetMouseState(&sMouseX, &sMouseY);
    sCursor = currentCursor;
}

void CursorMouse::loadSavedCursor()
{
    if (sMouseX == -1) {
        printf("Cursor has not been saved before");
        return;
    }

    SDL_WarpMouseInWindow(gWindow, sMouseX, sMouseY);
    setCursor(sCursor);
    //reset
    sMouseX = sMouseY = -1;
}

void CursorMouse::getAimPos(int &x, int &y)
{
    SDL_GetMouseState(&x, &y);
    x += getWidth() / 2;
    y += getHeight() / 2;
}

#endif