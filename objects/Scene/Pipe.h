#ifndef PIPE_H
#define PIPE_H

#include "Ground.h"
#include "../Other/ObjectsList.h"
#include "../Game/GameOver.h"

//pipe will be created when current time = this
Uint32 nextCreatedTime = waitTimeBeforePlaying;

class Pipe : public Object
{
private:
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

    static void spawnPipe(ObjectsList *pipeList);
    static void renderAll(ObjectsList *pipeList);
    static void resetTime();

    int getHeight() const override;

    bool render() override;
};

Pipe::Pipe(int height, bool flip)
{
    loadIMG();
    init(height, flip);
}

Pipe::~Pipe()
= default;

void Pipe::spawnPipe(ObjectsList *pipeList)
{
    if (SDL_GetTicks() >= nextCreatedTime) {
        //random flip
        bool flip = getRandomNumber(0, 1);
        //pipe height
        int pipeHeight = getRandomNumber(50, 300);
        //create new pipe
        Pipe *pipe = new Pipe(getRandomNumber(50, SCREEN_HEIGHT / 2), flip);
        pipeList->add(pipe);
    }
}

void Pipe::renderAll(ObjectsList *pipeList)
{
    if (!GameOver::gameIsOver()) {
        spawnPipe(pipeList);
    }

    pipeList->renderAll();
}

void Pipe::resetTime()
{
    nextCreatedTime = SDL_GetTicks() + waitTimeBeforePlaying;
}

bool Pipe::checkOutTheBorder()
{
    return (mPosX + getWidth() < 0);
}

void Pipe::init(int height, bool flip)
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH;
    if (flip) {
        mPosY = height - mTexture->getHeight();
        mHeight = mTexture->getHeight();
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
    mTexture->loadFromFile(sceneImagePath + "pipe.png");
}

int Pipe::getHeight() const
{
    return mHeight;
}

bool Pipe::render()
{
    mPosX -= gVelocityYScene;
    if (checkOutTheBorder()) {
        return false;
    }

    SDL_Rect rect;
    rect.x = 0, rect.y = 0;
    rect.h = mHeight, rect.w = getWidth();
    mTexture->render(mPosX, mPosY, &rect, 0, NULL, (mFlip) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

    return true;
}

#endif