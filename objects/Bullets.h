#ifndef BULLETS_H
#define BULLETS_H

#include "../LTexture.h"
#include "../utils.h"
#include "Pipe.h"

const int pivotX = 20;
const int pivotY = 10;

class Bullets;
//list of bullets
std::list<Bullets *> bulletsList;

class Bullets
{
private:
    LTexture mTexture;

    //The X and Y offsets
    int mPosX, mPosY;
    //The velocity
    int mVelX, mVelY;
    //Speed;
    double mSpeed;
    //Angle
    double mAngle;

    bool checkOutTheBorder();
    bool checkPipeCollision();
    bool move();

    void loadIMG();
    void calculateVelocity();

protected:
    void init(int x, int y, double angle);

public:
    Bullets(int x, int y, double angle);
    ~Bullets();

    static void add(int x, int y, double Angle);
    static void reset();
    static void renderAll();

    bool render();
};

Bullets::Bullets(int x, int y, double angle)
{
    init(x, y, angle);
    loadIMG();
}

Bullets::~Bullets()
{
    mTexture.free();
    // for (int i = 0; i < 2; i++) {
    //     mTexture[i].free();
    // }
}

void Bullets::add(int x, int y, double angle)
{
    //create bullet
    Bullets *bullet = new Bullets(x, y, angle);
    bulletsList.push_back(bullet);
}

void Bullets::reset()
{
    //clear bullets
    for (auto bullet : bulletsList) {
        delete bullet;
    }
    bulletsList.clear();
}

void Bullets::renderAll()
{
    //check collision before render
    for (auto bullet = bulletsList.begin(); bullet != bulletsList.end();) {
        if (!(*bullet)->render()) {
            delete (*bullet);
            bullet = bulletsList.erase(bullet);
        }
        else bullet++;
    }
}

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
    if (mPosX > SCREEN_WIDTH || mPosX + mTexture.getWidth() < 0) {
        return true;
    }
    if (mPosY > SCREEN_HEIGHT || mPosY + mTexture.getHeight() < 0) {
        return true;
    }

    return false;
}

bool Bullets::checkPipeCollision()
{
    for (auto pipe : pipeList) {
        if (checkCollision(mPosX, mPosY, mTexture.getWidth(), mTexture.getHeight(),
            pipe->getPosX(), pipe->getPosY(), pipe->getWidth(), pipe->getHeight())) {

            return true;
        }
    }

    return false;
}

void Bullets::loadIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-Bullets
    mTexture.loadFromFile(imagePath + "bullet0.png", true, 0, 0, 0);
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

    if (checkOutTheBorder() || checkPipeCollision()) return false;
    return true;
}

bool Bullets::render()
{
    if (move()) {
        mTexture.render(mPosX, mPosY, NULL, mAngle);
        return true;
    }

    return false;
}

#endif