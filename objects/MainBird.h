#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "Ground.h"
#include "Guns.h"
#include "Pipe.h"
#include "GameOver.h"

const int mainBirdPosX = 200;
const int mainBirdPosY = SCREEN_HEIGHT / 2;

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

    void loadIMG();
    void setPosToBorderPos();
    bool playingRender();
    bool dyingRender();

    bool isStayingOnGround();
    bool checkOutBorder();

    bool checkGroundCollision();
    bool checkPipeCollision();

public:
    MainBird(int x, int y);
    ~MainBird();


    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);

    bool move();
    bool render();
};

MainBird::MainBird(int x, int y)
{
    init(x, y);
    loadIMG();

    initGun();
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

    setGunPosAndAngle(x + mTexture[curIMGRender].getWidth() / 3, y + mTexture[curIMGRender].getHeight() * 2 / 3);
}

void MainBird::loadIMG()
{
    //image: https://flappybird.io/
    mTexture[0].loadFromFile(imagePath + "mainBird0.png", true, 124, 197, 205);
    mTexture[1].loadFromFile(imagePath + "mainBird1.png", true, 124, 197, 205);
    mTexture[2].loadFromFile(imagePath + "mainBird2.png", true, 124, 197, 205);
}

void MainBird::setPosToBorderPos()
{
    if (mPosX < 0) {
        mPosX = 0;
    }
    if (mPosX + mTexture[curIMGRender].getWidth() > SCREEN_WIDTH) {
        mPosX = SCREEN_WIDTH - mTexture[curIMGRender].getWidth();
    }
    if (mPosY + mTexture[curIMGRender].getHeight() < 0) {
        mPosY = -mTexture[curIMGRender].getHeight();
    }
}

bool MainBird::playingRender()
{
    if (move()) {
        //update pos for gun
        setGunPosAndAngle(mPosX + mTexture[curIMGRender].getWidth() / 3, mPosY + mTexture[curIMGRender].getHeight() / 3);
    }
    if (checkPipeCollision()) {
        GameOver::onGameOver(true);
    }
    //change wing flapping speed
    if (isStayingOnGround()) {
        imgChangeVel = 5;
    }
    else {
        imgChangeVel = 2;
    }

    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    //renderGun();

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

bool MainBird::dyingRender()
{
    //start falling
    if (!died) {
        died = true;

        //continue moving
        mVelX = gVelocityYScene;
        mVelY = 15;
        limitAngleLower = 90;
    }

    if (mVelX > 0 && isStayingOnGround()) {
        //Velocity decreases due to friction
        mVelX--;
    }

    bool moved = move();
    if (moved) {
        //update pos for gun
        setGunPosAndAngle(mPosX + mTexture[curIMGRender].getWidth() / 3, mPosY + mTexture[curIMGRender].getHeight() / 3);
    }
    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    return moved;
}

bool MainBird::isStayingOnGround()
{
    return (mPosY + mTexture[curIMGRender].getHeight() == groundPosY + 10);
}

bool MainBird::checkGroundCollision()
{
    if (mPosY + mTexture[curIMGRender].getHeight() >= groundPosY + 10) {
        return true;
    }
    return false;
}

bool MainBird::checkPipeCollision()
{
    for (auto pipe : pipeList) {
        if (checkCollision(pipe->getPosX(), pipe->getPosY(), pipe->getWidth(), pipe->getHeight(),
            mPosX, mPosY, mTexture[curIMGRender].getWidth(), mTexture[curIMGRender].getHeight())) {

            return true;
        }
    }

    return false;
}

bool MainBird::checkOutBorder()
{
    if (mPosX < 0) {
        return true;
    }
    if (mPosX + mTexture[curIMGRender].getWidth() > SCREEN_WIDTH) {
        return true;
    }
    if (mPosY + mTexture[curIMGRender].getHeight() < 0) {
        return true;
    }
    return false;
}

void MainBird::handleEvent(SDL_Event *e)
{
    if (GameOver::gameIsOver()) {
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch (e->key.keysym.sym) {
        case SDLK_SPACE:
            mVelY = 15;
            mVelAngle = 8;
            break;

        default:
            break;
        }
    }
    else {
        handleGunEvent(e);
    }
}

void MainBird::handleKey(const Uint8 *currentKeyStates)
{
    if (GameOver::gameIsOver()) {
        return;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        mVelX = -5;
    }
    else if (currentKeyStates[SDL_SCANCODE_D]) {
        mVelX = 5;
    }
    else mVelX = 0;
}

bool MainBird::move()
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

    if (checkGroundCollision()) {
        //not staying on the ground
        if (!isStayingOnGround()) {
            //set pos to ground pos
            mPosY = groundPosY + 10 - mTexture[curIMGRender].getHeight();
            moved = true;
        }
        else {
            //continue to fall
            mPosY += mVelY;
        }
        //reset Y velocity
        mVelY = 0;
    }
    else {
        moved = true;
    }

    if (checkOutBorder()) {
        // set pos to border pos
        setPosToBorderPos();
        if (isStayingOnGround()) {
            //reset Y velocity
            mVelY = 0;
        }
    }

    return moved;
}

bool MainBird::render()
{
    bool rendered = true;
    if (!GameOver::gameIsOver()) {
        playingRender();
    }
    if (GameOver::gameIsOver()) {
        rendered = dyingRender();
    }

    if (!isStayingOnGround()) {
        mVelY--;
        //Decrease Y velocity because of gravitation
    }
    mVelAngle -= 0.5;

    return rendered;
}

#endif