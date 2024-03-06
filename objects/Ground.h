#ifndef GROUND_H
#define GROUND_H

#include "../LTexture.h"

const int groundPosX = 0;
const int groundPosY = SCREEN_HEIGHT - 79;

class Ground
{
private:
    LTexture mTexture;

    //The X and Y offsets
    int mPosX, mPosY;

    void init(int x, int y);
    void loadIMG();
public:
    Ground(int x, int y);
    ~Ground();

    void render();
};

Ground::Ground(int x, int y)
{
    init(x, y);
    loadIMG();
}

Ground::~Ground()
{
    mTexture.free();
}

void Ground::init(int x, int y)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Create the necessary SDL_Rects
    //mColliders.resize(11);
}

void Ground::loadIMG()
{
    //image: https://flappybird.io/
    mTexture.loadFromFile(imagePath + "ground.png");
}

void Ground::render()
{
    int tWidth = mTexture.getWidth();
    mTexture.render(mPosX, mPosY);
    mTexture.render(mPosX + tWidth, mPosY);
    mTexture.render(mPosX + 2 * tWidth, mPosY);

    mPosX -= gVelocityYScene; // Adjust the speed of scrolling as needed

    // If the ground has reached the left edge, reset its position
    if (mPosX <= -mTexture.getWidth()) {
        mPosX = 0;
    }
}

#endif