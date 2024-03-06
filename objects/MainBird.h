#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "../LTexture.h"
#include "Ground.h"
#include <math.h>

const int mainBirdPosX = 5;
const int mainBirdPosY = 350;

class MainBird
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
    //The angel
    double mAngle;

    void init(int x, int y);
    void loadIMG();
    void turnTowards(int mouseX, int mouseY);

    bool checkCollision(const Ground& ground);
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

void MainBird::turnTowards(int mouseX, int mouseY)
{
    double dx = mouseX - mPosX;
    double dy = mouseY - mPosY;
    double angle = atan2(dy, dx) * 180 / M_PI;

    mAngle = angle;
}

bool MainBird::checkCollision(const Ground& ground)
{
    if (mPosY + mTexture[curIMGRender].getHeight() > groundPosY + 10) {
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
    else if (e.type == SDL_MOUSEMOTION) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        turnTowards(mouseX, mouseY);
    }
}

void MainBird::move(const Ground& ground)
{
    mPosY -= mVelY;
    if (checkCollision(ground)) {
        //stay on the ground
        int t = mTexture[curIMGRender].getHeight();
        mPosY = groundPosY + 10 - mTexture[curIMGRender].getHeight();
        //reset Y velocity
        mVelY = 0;
    }
}

void MainBird::render()
{
    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);

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