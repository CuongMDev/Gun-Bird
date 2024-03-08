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
    //image: https://www.freepik.com/free-vector/nature-roadside-background-scene_40169781.htm#query=pixel%20sky%20background&position=2&from_view=keyword&track=ais&uuid=139c01bd-968c-4c3e-9f4f-1612c7cd6e5f
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