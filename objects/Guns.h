#ifndef GUNS_H
#define GUNS_H

#include "Bullets.h"
#include <math.h>
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
    //list of bullets
    std::list<Bullets*> bulletsList;

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
    void shoot();

protected:
    void initGun(int x, int y);
    void renderGun();
    void handleGunEvent(SDL_Event e);
    void updateGunPosAndAngle(int x, int y);

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

void Guns::initGun(int x, int y)
{
    loadGunIMG();
    updateGunPosAndAngle(x, y);
}

void Guns::loadGunIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    mTexture.loadFromFile(imagePath + "gun0.png", true, 67, 76, 111);
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
    int bulletMPosX = mPosX;
    if (mAngle > 40) {
        bulletMPosX += mTexture.getWidth() / 2;
    }
    //create bullet
    Bullets* bullet = new Bullets(bulletMPosX, mPosY, mAngle);
    //recoil
    mouseRecoil(bulletsList.size(), 5 * bulletsList.size());
    //add to list
    bulletsList.push_back(bullet);
}

void Guns::handleGunEvent(SDL_Event e)
{
    if (e.type == SDL_MOUSEMOTION) {
        updateAngle();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        shoot();
    }
}

void Guns::renderGun()
{
    mTexture.render(mPosX, mPosY, NULL, mAngle);
    //render bullets
    for (auto bullet = bulletsList.begin(); bullet != bulletsList.end();) {
        if (!(*bullet)->render()) {
            delete (*bullet);
            bullet = bulletsList.erase(bullet);
        }
        else bullet++;
    }
}

void Guns::updateGunPosAndAngle(int x, int y)
{
    mPosX = x;
    mPosY = y;
    updateAngle();
}

#endif