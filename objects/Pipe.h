#ifndef PIPE_H
#define PIPE_H

#include "../LTexture.h"
#include "Ground.h"

const Uint32 waitTimeBeforePlaying = 2000;

class Pipe;
static std::list<Pipe *> pipeList;
//pipe will be created when current time = this
Uint32 nextCreatedTime = waitTimeBeforePlaying;

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

    static void renderAll();
    static void reset();

    int getPosX();
    int getPosY();
    int getHeight();
    int getWidth();

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

void Pipe::renderAll()
{
    //create pipe;
    if (SDL_GetTicks() >= nextCreatedTime) {
        //random flip
        bool flip = getRandomNumber(0, 1);
        //pipe height
        int pipeHeight = getRandomNumber(50, 300);
        //create new pipe
        Pipe *pipe = new Pipe(getRandomNumber(50, SCREEN_HEIGHT / 2), flip);
        pipeList.push_back(pipe);

        //reset created time
        nextCreatedTime = SDL_GetTicks() + getRandomNumber(500, 500);
    }

    //render all pipes
    for (auto pipe = pipeList.begin(); pipe != pipeList.end();) {
        if (!(*pipe)->render()) {
            delete (*pipe);
            pipe = pipeList.erase(pipe);
        }
        else pipe++;
    }
}

void Pipe::reset()
{
    //clear pipes
    for (auto pipe : pipeList) {
        delete pipe;
    }
    pipeList.clear();
    nextCreatedTime = SDL_GetTicks() + waitTimeBeforePlaying;
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

inline int Pipe::getPosX()
{
    return mPosX;
}

inline int Pipe::getPosY()
{
    return mPosY;
}

inline int Pipe::getHeight()
{
    return mHeight;
}

inline int Pipe::getWidth()
{
    return mTexture.getWidth();
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