#ifndef BULLETS_H
#define BULLETS_H

#include "../LTexture.h"
#include "../utils.h"
#include "Pipe.h"
#include "ObjectsList.h"

const int pivotX = 20;
const int pivotY = 10;

class Bullets : public Object
{
private:
    //The velocity
    int mVelX, mVelY;
    //Speed;
    double mSpeed;
    //Angle
    double mAngle;

    bool checkOutTheBorder();
    bool move();

    void loadIMG();
    void calculateVelocity();

protected:
    void init(int x, int y, double angle);

public:
    Bullets(int x, int y, double angle);
    ~Bullets();

    bool render() override;
};

Bullets::Bullets(int x, int y, double angle)
{
    init(x, y, angle);
    loadIMG();
}

Bullets::~Bullets()
=default;

void Bullets::init(int x, int y, double angle)
{
    mPosX = x + pivotX / 2;
    mPosY = y + pivotY / 2;

    mSpeed = 20;

    mAngle = angle;

    calculateVelocity();
}

bool Bullets::checkOutTheBorder()
{
    if (mPosX > SCREEN_WIDTH || mPosX + getWidth() < 0) {
        return true;
    }
    if (mPosY > SCREEN_HEIGHT || mPosY + getHeight() < 0) {
        return true;
    }

    return false;
}

void Bullets::loadIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-Bullets
    mTexture->loadFromFile(imagePath + "bullet0.png", true, 0, 0, 0);
}

inline void Bullets::calculateVelocity()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    double dx = mouseX - mPosX;
    double dy = mouseY - mPosY;

    double length = distance(mPosX, mPosY, mouseX, mouseY);
    if (length != 0) {
        dx /= length;
        dy /= length;
    }

    // Scale direction vector by speed to get velocity vector
    mVelX = (dx * mSpeed) + trunc(2 * (double)(SCREEN_HEIGHT - mouseY) / SCREEN_HEIGHT);
    mVelY = dy * mSpeed;
}

bool Bullets::move()
{
    mPosX += mVelX;
    mPosY += mVelY;

    if (checkOutTheBorder()) return false;
    return true;
}

bool Bullets::render()
{
    if (move()) {
        mTexture->render(mPosX, mPosY, NULL, mAngle);
        return true;
    }

    return false;
}

#endif