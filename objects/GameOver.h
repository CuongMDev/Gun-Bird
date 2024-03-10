#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "../LTexture.h"

const int gameOverLoadSpeed = 10;
const int disBetweenHomeAndRetry = 20;
const int addWidthButton = 20;
const int addHeightButton = 20;

enum BUTTON
{
    NONE,
    HOMEBUTTON,
    RETRYBUTTON
};

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
    //increase size of image
    int addWidth[IMAGE_COUNT], addHeight[IMAGE_COUNT];

    static bool isOver;
    void loadIMG();
    bool checkCollisionButton(BUTTON button);
public:
    GameOver();
    ~GameOver();

    static bool gameIsOver();
    static void onGameOver(bool value);

    void init();
    void initPos();
    void render();
    void reset();

    BUTTON handleEvent(SDL_Event *e);
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
    initPos();
    mCurWidth = 0;
}

void GameOver::initPos() {
    mPosX[GAMEOVER] = SCREEN_WIDTH / 2 - mTexture[GAMEOVER].getWidth() / 2;
    mPosY[GAMEOVER] = SCREEN_HEIGHT / 2 - mTexture[GAMEOVER].getHeight();

    mPosX[HOME] = SCREEN_WIDTH / 2 - mTexture[HOME].getWidth() - disBetweenHomeAndRetry / 2;
    mPosY[HOME] = SCREEN_HEIGHT / 2 + mTexture[HOME].getHeight() / 3;

    mPosX[RETRY] = SCREEN_WIDTH / 2 + disBetweenHomeAndRetry;
    mPosY[RETRY] = SCREEN_HEIGHT / 2 + mTexture[RETRY].getHeight() / 3;

    for (int i = 0; i < IMAGE_COUNT; i++) {
        addHeight[i] = 0;
        addWidth[i] = 0;
    }
}

void GameOver::loadIMG()
{
    //image: https://www.dreamstime.com/pixel-game-over-text-image-bit-assets-cross-stitch-pattern-t-shirt-design-vector-illustration-image222135423
    mTexture[GAMEOVER].loadFromFile(imagePath + "gameover.png");
    mTexture[HOME].loadFromFile(imagePath + "home.png");
    mTexture[RETRY].loadFromFile(imagePath + "retry.png");
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
    else {
        mCurWidth = mTexture[GAMEOVER].getWidth();

        mTexture[HOME].render(mPosX[HOME], mPosY[HOME], NULL, 0, NULL, SDL_FLIP_NONE, addWidth[HOME], addHeight[HOME]);
        mTexture[RETRY].render(mPosX[RETRY], mPosY[RETRY], NULL, 0, NULL, SDL_FLIP_NONE, addWidth[RETRY], addHeight[RETRY]);
    }
}

void GameOver::reset()
{
    mCurWidth = 0;
    isOver = false;
}

BUTTON GameOver::handleEvent(SDL_Event *e)
{
    BUTTON buttonCollided = NONE;
    if (checkCollisionButton(HOMEBUTTON)) {
        buttonCollided = HOMEBUTTON;
    }
    else if (checkCollisionButton(RETRYBUTTON)) {
        buttonCollided = RETRYBUTTON;
    }

    if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            // left mouse button pressed
            return buttonCollided;
        }
    }

    return NONE;
}

bool GameOver::checkCollisionButton(BUTTON button)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (checkCollision(mouseX, mouseY, 0, 0, mPosX[button], mPosY[button], mTexture[button].getWidth(), mTexture[button].getHeight())) {
        addWidth[button] = addWidthButton;
        addHeight[button] = addHeightButton;

        return true;
    }
    else {
        //reset;
        addWidth[button] = 0;
        addHeight[button] = 0;

        return false;
    }
}

#endif //GAMEOVER_H
