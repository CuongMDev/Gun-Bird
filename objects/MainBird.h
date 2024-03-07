#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "../LTexture.h"
#include "Ground.h"
#include "Guns.h"

const int mainBirdPosX = 5;
int mainBirdPosY = SCREEN_HEIGHT / 2;

class MainBird :private Guns
{
private:
    LTexture mTexture[3];

    int curTimeRender;
    int curIMGRender;
    int imgChangeVel;
    //The X and Y offsets
    int mPosX, mPosY;
    //The velocity
    int mVelX, mVelY;
    //The angle
    double mAngle;

    void init(int x, int y);
    void loadIMG();

    bool isStayingOnGround();
    bool checkCollision(const Ground& ground);
    bool checkOutBorder();
public:
    MainBird(int x, int y);
    ~MainBird();

    void handleEvent(SDL_Event e);
    void move(const Ground& ground);
    void render();
};

MainBird::MainBird(int x, int y)
{
    init(x, y);
    loadIMG();

    initGun(x + mTexture[curIMGRender].getWidth() / 3, y + mTexture[curIMGRender].getHeight() * 2 / 3);
}

MainBird::~MainBird()
{
    for (int i = 0; i < 2; i++) {
        mTexture[i].free();
    }
}

void MainBird::init(int x, int y)
{
    curTimeRender = 0;
    curIMGRender = 0;
    imgChangeVel = 5;

    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    mAngle = 0;
    //Create the necessary SDL_Rects
    //mColliders.resize(11);

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void MainBird::loadIMG()
{
    //image: https://flappybird.io/
    mTexture[0].loadFromFile(imagePath + "mainBird0.png", true, 124, 197, 205);
    mTexture[1].loadFromFile(imagePath + "mainBird1.png", true, 124, 197, 205);
    mTexture[2].loadFromFile(imagePath + "mainBird2.png", true, 124, 197, 205);
}

bool MainBird::isStayingOnGround()
{
    return (mPosY + mTexture[curIMGRender].getHeight() == groundPosY + 10);
}

bool MainBird::checkCollision(const Ground& ground)
{
    if (mPosY + mTexture[curIMGRender].getHeight() > groundPosY + 10) {
        return true;
    }
    return false;
}

bool MainBird::checkOutBorder()
{
    if (mPosY + mTexture[curIMGRender].getHeight() < 0) {
        return true;
    }
    return false;
}

void MainBird::handleEvent(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch (e.key.keysym.sym) {
        case SDLK_SPACE:
            mVelY = 15;
        }
    }
    else {
        handleGunEvent(e);
    }
}

void MainBird::move(const Ground& ground)
{
    mPosY -= mVelY;

    if (checkOutBorder()) {
        mPosY = -mTexture[curIMGRender].getHeight();
        //reset Y velocity
        mVelY = 0;
    }

    if (checkCollision(ground)) {
        //not staying on the ground
        if (!isStayingOnGround()) {
            mPosY = groundPosY + 10 - mTexture[curIMGRender].getHeight();
            updateGunPosAndAngle(mPosX + mTexture[curIMGRender].getWidth() / 3, mPosY + mTexture[curIMGRender].getHeight() / 3);
        }
        else {
            mPosY += mVelY;
        }
        //reset Y velocity
        mVelY = 0;
    }
    else {
        //update gun angle if bird is moving
        updateGunPosAndAngle(mPosX + mTexture[curIMGRender].getWidth() / 3, mPosY + mTexture[curIMGRender].getHeight() / 3);
    }
}

void MainBird::render()
{
    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    renderGun();

    //Decrease Y velocity because of gravitation
    mVelY--;
    curTimeRender++;
    if (curTimeRender == imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender == 3) {
            curIMGRender = 0;
        }
    }
}

#endif