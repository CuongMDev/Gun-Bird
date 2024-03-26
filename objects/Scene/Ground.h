#ifndef GROUND_H
#define GROUND_H

#include "../Other/Object.h"

const int groundPosX = 0;
const int groundPosY = SCREEN_HEIGHT - 79;

class Ground : public Object
{
private:
    void init(int x, int y);
    void loadIMG();

    bool updateState() override;
    void renderTogRenderer() override;
public:
    Ground(int x, int y);
    ~Ground();
};

Ground::Ground(int x, int y)
{
    init(x, y);
    loadIMG();
}

Ground::~Ground()
= default;

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
    //image: https://www.freepik.com/free-vector/game-seamless-ground-texture-layer-cartoon-vector-grass-land-desert-surface-gravel-pattern-ice-floor-lava-formation-environment-illustration-set-mobile-app-underground-stone-ui-path_67808349.htm#query=2d%20ground&position=0&from_view=keyword&track=ais&uuid=1be95183-12b3-4e28-8e24-4604b89d1b8e
    mTexture->loadFromFile(sceneImagePath + "ground.png");
}

bool Ground::updateState()
{
    mPosX -= gVelocityYScene;
    // If the ground has reached the left edge, reset its position
    if (mPosX <= -mTexture->getWidth()) {
        mPosX = 0;
    }

    return true;
}

void Ground::renderTogRenderer()
{
    int tWidth = getWidth();
    mTexture->render(mPosX, mPosY);
    mTexture->render(mPosX + tWidth, mPosY);
    mTexture->render(mPosX + 2 * tWidth, mPosY);
}

#endif