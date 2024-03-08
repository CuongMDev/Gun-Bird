#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "../LTexture.h"

const int gameOverLoadSpeed = 10;

class GameOver {
private:
    enum IMAGE
    {
        GAMEOVER,
        HOME,
        RETRY,
        IMAGE_COUNT
    };

    LTexture mTexture[IMAGE_COUNT];

    //The X and Y offsets
    int mPosX[IMAGE_COUNT], mPosY[IMAGE_COUNT];
    //use to create animation
    int mCurWidth;

    static bool isOver;
    void loadIMG();
public:
    GameOver();
    ~GameOver();

    static bool gameIsOver();
    static void onGameOver(bool value);

    void init();
    void render();
    void reset();
};

bool GameOver::isOver = false;

GameOver::GameOver()
{
    loadIMG();
    init();
}

GameOver::~GameOver()
{
    for (int i = 0; i < IMAGE_COUNT; i++) {
        mTexture[i].free();
    }
}

bool GameOver::gameIsOver() {
    return isOver;
}

void GameOver::onGameOver(bool value) {
    isOver = value;
}

void GameOver::init()
{
    //Initialize the offsets
    mPosX[GAMEOVER] = SCREEN_WIDTH / 2 - mTexture[GAMEOVER].getWidth() / 2;
    mPosY[GAMEOVER] = SCREEN_HEIGHT / 2 - mTexture[GAMEOVER].getHeight() / 2;

    mCurWidth = 0;
}

void GameOver::loadIMG()
{
    //image: https://www.dreamstime.com/pixel-game-over-text-image-bit-assets-cross-stitch-pattern-t-shirt-design-vector-illustration-image222135423
    mTexture[GAMEOVER].loadFromFile(imagePath + "gameover.png");
}

void GameOver::render()
{
    SDL_Rect clip;
    clip.x = 0; clip.y = 0;
    clip.w = mCurWidth, clip.h = mTexture[GAMEOVER].getHeight();

    mTexture[GAMEOVER].render(mPosX[GAMEOVER], mPosY[GAMEOVER], &clip);

    //continue to load
    if (mCurWidth + gameOverLoadSpeed <= mTexture[GAMEOVER].getWidth()) {
        mCurWidth += gameOverLoadSpeed;
    }
    else mCurWidth = mTexture[GAMEOVER].getWidth();
}

void GameOver::reset()
{
    mCurWidth = 0;
    isOver = false;
}

#endif //GAMEOVER_H
