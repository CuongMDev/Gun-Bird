#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "../LTexture.h"
#include "Ground.h"
#include "Guns.h"
#include "Pipe.h"

const int mainBirdPosX = 200;
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
    //The angle
    double mAngle;
    //The velocity
    int mVelX, mVelY;
    double mVelAngle;

    double limitAngleUpper;
    double limitAngleLower;

    //check whether bird died
    bool died;

    void init(int x, int y);
    void loadIMG();
    bool playingRender(const Ground& ground, const std::list<Pipe*>& pipeList);
    bool dyingRender(const Ground& ground);

    bool isStayingOnGround();
    bool checkOutBorder();

    bool checkCollision(const Ground& ground);
    bool checkCollision(const std::list<Pipe*>& pipeList);

public:
    MainBird(int x, int y);
    ~MainBird();

    void handleEvent(SDL_Event e);

    bool move(const Ground& ground);
    bool render(const Ground& ground, const std::list<Pipe*>& pipeList);
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
    died = false;

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
    mVelAngle = 0;

    limitAngleUpper = -45;
    limitAngleLower = 0;
}

void MainBird::loadIMG()
{
    //image: https://flappybird.io/
    mTexture[0].loadFromFile(imagePath + "mainBird0.png", true, 124, 197, 205);
    mTexture[1].loadFromFile(imagePath + "mainBird1.png", true, 124, 197, 205);
    mTexture[2].loadFromFile(imagePath + "mainBird2.png", true, 124, 197, 205);
}

bool MainBird::playingRender(const Ground& ground, const std::list<Pipe*>& pipeList)
{
    move(ground);
    if (checkCollision(pipeList)) {
        gameOver = true;
    }
    //change wing flapping speed
    if (isStayingOnGround()) {
        imgChangeVel = 5;
    }
    else {
        imgChangeVel = 2;
    }

    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    renderGun();

    //increase time render
    curTimeRender++;
    if (curTimeRender >= imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender == 3) {
            curIMGRender = 0;
        }
    }

    return true;
}

bool MainBird::dyingRender(const Ground& ground)
{
    //start falling
    if (!died) {
        died = true;

        //continue moving
        mVelX = gVelocityYScene;
        mVelY = 15;
        gVelocityYScene = 0;
        limitAngleLower = 90;
    }

    if (mVelX > 0 && isStayingOnGround()) {
        //Velocity decreases due to friction
        mVelX--;
    }

    bool moved = move(ground);
    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    return moved;
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

bool MainBird::checkCollision(const std::list<Pipe*>& pipeList)
{
    for (auto pipe : pipeList) {
        if (pipe->getPosX() <= mPosX + mTexture[curIMGRender].getWidth() && mPosX <= pipe->getPosX() + pipe->getWitdh()
            && pipe->getPosY() <= mPosY + mTexture[curIMGRender].getHeight() && mPosY <= pipe->getPosY() + pipe->getHeight()) {

            return true;
        }
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
    if (gameOver) {
        return;
    }
    if (e.type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch (e.key.keysym.sym) {
        case SDLK_SPACE:
            mVelY = 15;
            mVelAngle = 8;
        }
    }
    else {
        handleGunEvent(e);
    }
}

bool MainBird::move(const Ground& ground)
{
    bool moved = false;
    if (mVelX) moved = true;

    mPosX += mVelX;
    mPosY -= mVelY;
    mAngle -= mVelAngle;

    if (mAngle < limitAngleUpper) {
        mAngle = limitAngleUpper;
    }
    //Do not lower head
    if (mAngle > limitAngleLower) {
        mAngle = limitAngleLower;
        mVelAngle = 0;
    }

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
            moved = true;
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
        moved = true;
    }

    return moved;
}

bool MainBird::render(const Ground& ground, const std::list<Pipe*>& pipeList)
{
    bool rendered = false;
    if (!gameOver) {
        rendered = playingRender(ground, pipeList);
    }
    else rendered = dyingRender(ground);

    //Decrease Y velocity because of gravitation
    mVelY--;
    mVelAngle -= 0.5;

    return rendered;
}

#endif