#include "LTexture.h"

class MainBird
{
private:
    LTexture mTexture[3];

    int curIMGRender;
    int imgChangeVel;
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    //The velocity of the dot
    int mVelX, mVelY;

    void init(int x, int y);
    void loadIMG();
    void render(int x, int y);
public:
    MainBird(int x, int y);
    ~MainBird();
};

MainBird::MainBird(int x, int y)
{
    init(x, y);
    loadIMG();
}

MainBird::~MainBird()
{
}

void MainBird::init(int x, int y)
{
    curIMGRender = 0;
    imgChangeVel = 0;

    //Initialize the offsets
    mPosX = x;
    mPosY = y;

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

void MainBird::render(int x, int y)
{
    mTexture[curIMGRender / imgChangeVel].render(mPosX, mPosY);

    curIMGRender++;
    if (curIMGRender / imgChangeVel == 3) {
        curIMGRender = 0;
    }
}