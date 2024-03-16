#ifndef GUNS_H
#define GUNS_H

#include "Bullets.h"
#include "../CursorMouse/CursorMouse.h"
#include <cmath>
#include <list>

const int gunPosX = 5;
const int gunPosY = groundPosY + 10;

class Guns : private Object
{
private:
    ObjectsList bulletsList;

    //The angle
    double mAngle;

    //shot when curTime=speed
    int curTime;
    int shootDelay;

    int mVelRecoil;
    const int maxVelRecoil = 10;

    bool mouseHold;

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
    //check and shoot
    void shoot();

public:

    Guns();

    ObjectsList &getBulletList();

    void init();
    bool render() override;
    void handleEvent(SDL_Event *e);
    void setPosAndAngle(int x, int y);
};

Guns::Guns()
{
    loadGunIMG();
    init();
}

ObjectsList &Guns::getBulletList()
{
    return bulletsList;
}

void Guns::init()
{
    mPosX = 0;
    mPosY = 0;
    mAngle = 0;
    mVelRecoil = 0;
    shootDelay = 5;
    mouseHold = false;

    bulletsList.reset();
}

void Guns::loadGunIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    mTexture->loadFromFile(gunImagePath + "ak47.png", true, 67, 76, 111);
}

void Guns::turnTowards(int mouseX, int mouseY)
{
    double dx = mouseX - mPosX;
    double dy = mouseY - mPosY;
    double angle = atan2(dy, dx) * 180 / M_PI;

    mAngle = angle;
}

void Guns::updateAngle()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    turnTowards(mouseX, mouseY);
}

void Guns::shoot()
{
    if (!mouseHold) {
        if (mVelRecoil > 0) {
            mVelRecoil--;
            //restore gun aim
            CursorMouse::move(0, 2);
        }
        return;
    }

    curTime++;
    if (curTime < shootDelay) {
        //It's not time to shoot yet
        return;
    }

    if (mVelRecoil < maxVelRecoil) {
        mVelRecoil++;
    }
    //reset
    curTime = 0;
    //recoil
    CursorMouse::recoilMouse(curTime, 10 * std::max(0, mVelRecoil - 3));
    //add bullet
    bulletsList.add(new Bullets(mPosX, mPosY, mAngle));
}

void Guns::handleEvent(SDL_Event *e)
{
    if (e->type == SDL_MOUSEMOTION) {
        updateAngle();
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            //left mouse
            mouseHold = true;
        }
    }
    else if (e->type == SDL_MOUSEBUTTONUP) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            //left mouse
            mouseHold = false;
        }
    }
}

bool Guns::render()
{
    shoot();
    bulletsList.renderAll();

    SDL_Point center{ pivotX, pivotY };
    mTexture->render(mPosX, mPosY, NULL, mAngle, &center);

    return true;
}

void Guns::setPosAndAngle(int x, int y)
{
    mPosX = x;
    mPosY = y;
    updateAngle();
}

#endif