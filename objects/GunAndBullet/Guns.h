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

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
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
    mPosX = 0;
    mPosY = 0;
    mAngle = 0;
}

ObjectsList &Guns::getBulletList()
{
    return bulletsList;
}

void Guns::init()
{
    loadGunIMG();
    bulletsList.reset();
}

void Guns::loadGunIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    mTexture->loadFromFile(gunImagePath + "gun0.png", true, 67, 76, 111);
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
    cursorMouse->recoilMouse(bulletsList.count (), 5 * bulletsList.count());
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
            // left mouse button pressed
            shoot();
        }
    }
}

bool Guns::render()
{
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