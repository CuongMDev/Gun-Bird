#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../Other/Object.h"

const int backgroundPosX = 0;
const int backgroundPosY = 0;

class Background : public Object
{
private:
    void init();
    void loadIMG();

    bool updateState() override;
    void renderTogRenderer() override;
public:
    Background();
    ~Background();
};

Background::Background()
{
    init();
    loadIMG();
}

Background::~Background()
= default;

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
    mTexture->loadFromFile(sceneImagePath + "background.png");
}

bool Background::updateState()
{
    mPosX -= gVelocityYScene;
    // If the ground has reached the left edge, reset its position
    if (mPosX <= -mTexture->getWidth()) {
        mPosX = 0;
    }

    return true;
}

void Background::renderTogRenderer()
{
    int tWidth = mTexture->getWidth();
    mTexture->render(mPosX, mPosY);
    mTexture->render(mPosX + tWidth, mPosY);
    mTexture->render(mPosX + 2 * tWidth, mPosY);
}

#endif