#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../LTexture.h"

const int backgroundPosX = 0;
const int backgroundPosY = 0;

class Background
{
private:
    LTexture mTexture;

    //The X and Y offsets
    int mPosX, mPosY;


    void init();
    void loadIMG();
public:
    Background();
    ~Background();

    void render();
};

Background::Background()
{
    init();
    loadIMG();
}

Background::~Background()
{
    mTexture.free();
}

void Background::init()
{
    //Initialize the offsets
    mPosX = backgroundPosX;
    mPosY = backgroundPosY;

    //Create the necessary SDL_Rects
    //mColliders.resize(11);
}

void Background::loadIMG()
{
    //image: https://vi.pngtree.com/freebackground/forest-landscape-horizontal-seamless-background-for-games-apps_1800315.html
    mTexture.loadFromFile(imagePath + "background.png");
}

void Background::render()
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