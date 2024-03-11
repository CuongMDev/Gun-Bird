#ifndef GUNS_H
#define GUNS_H

#include "Bullets.h"
#include "../CursorMouse.h"
#include <cmath>
#include <list>

const int gunPosX = 5;
const int gunPosY = groundPosY + 10;

class Guns
{
private:
    LTexture mTexture;

    //The X and Y offsets
    int mPosX, mPosY;
    //The angle
    double mAngle;

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
    void shoot();

protected:
    void initGun();
    void renderGun();
    void handleGunEvent(SDL_Event *e);
    void setGunPosAndAngle(int x, int y);

    Guns();
    ~Guns();
};

Guns::Guns()
{
    mPosX = 0;
    mPosY = 0;
    mAngle = 0;
}

Guns::~Guns()
{
    mTexture.free();
    // for (int i = 0; i < 2; i++) {
    //     mTexture[i].free();
    // }
}

void Guns::initGun()
{
    loadGunIMG();
}

void Guns::loadGunIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    mTexture.loadFromFile(gunImagePath + "gun0.png", true, 67, 76, 111);
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
    //recoil
    cursorMouse->recoilMouse(bulletsList.size(), 5 * bulletsList.size());
    //create bullet
    Bullets::add(mPosX, mPosY, mAngle);
}

void Guns::handleGunEvent(SDL_Event *e)
{
    if (e->type == SDL_MOUSEMOTION) {
        updateAngle();
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            // left mouse button pressed
            shoot();
        }
    }
}

void Guns::renderGun()
{
    Bullets::renderAll();

    SDL_Point center{ pivotX, pivotY };
    mTexture.render(mPosX, mPosY, NULL, mAngle, &center);
}

void Guns::setGunPosAndAngle(int x, int y)
{
    mPosX = x;
    mPosY = y;
    updateAngle();
}

#endif