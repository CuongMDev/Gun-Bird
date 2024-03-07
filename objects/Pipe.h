#ifndef PIPE_H
#define PIPE_H

#include "../LTexture.h"

class Pipe
{
private:
    LTexture mTexture;

    //The X and Y offsets
    int mPosX, mPosY;
    //pipe height
    int mHeight;
    //Is flipped
    bool mFlip;

    bool checkOutTheBorder();

    void init(int height, bool flip);
    void loadIMG();
public:
    Pipe(int height, bool flip);
    ~Pipe();

    bool render();
};

Pipe::Pipe(int height, bool flip)
{
    loadIMG();
    init(height, flip);
}

Pipe::~Pipe()
{
    mTexture.free();
}

bool Pipe::checkOutTheBorder()
{
    return (mPosX + mTexture.getWidth() < 0);
}

void Pipe::init(int height, bool flip)
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH;
    if (flip) {
        mPosY = height - mTexture.getHeight();
        mHeight = mTexture.getHeight();
    }
    else {
        mPosY = groundPosY - height;// SCREEN_HEIGHT - height - (SCREEN_HEIGHT - groundPosY);
        mHeight = height;
    }

    //Create the necessary SDL_Rects
    mFlip = flip;
}

void Pipe::loadIMG()
{
    //image: https://github.com/terroo/flappybird/tree/main
    mTexture.loadFromFile(imagePath + "pipe.png");
}

bool Pipe::render()
{
    mPosX -= gVelocityYScene;
    if (checkOutTheBorder()) {
        return false;
    }

    SDL_Rect rect;
    rect.x = 0, rect.y = 0;
    rect.h = mHeight, rect.w = mTexture.getWidth();
    mTexture.render(mPosX, mPosY, &rect, 0, NULL, (mFlip) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

    return true;
}

#endif