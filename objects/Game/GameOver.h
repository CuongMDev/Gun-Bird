#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "../Other/LTexture.h"

const int gameOverLoadSpeed = 10;
const int disBetweenHomeAndRetry = 20;
const int addScaleButton = 10;

enum BUTTON
{
    NONE,
    HOMEBUTTON,
    RETRYBUTTON,
    CONTINUE_BUTTON,

    BUTTON_COUNT
};

class GameOver {
private:
    enum IMAGE
    {
        GAMEOVER,
        HOME,
        RETRY,
        CONTINUE,

        IMAGE_COUNT
    };

    LTexture mTexture[IMAGE_COUNT];

    //The X and Y offsets
    int mPosX[IMAGE_COUNT], mPosY[IMAGE_COUNT];
    //use to create animation
    int loadGameOverState;
    //increase size of image
    int addScale[IMAGE_COUNT];

    static bool isOver;
    void loadIMG();
    bool checkCollisionButton(BUTTON button);
public:
    GameOver();
    ~GameOver();

    static bool gameIsOver();
    static void onGameOver();

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

void GameOver::onGameOver() {
    isOver = true;
}

void GameOver::init()
{
    initPos();
    loadGameOverState = 0;
}

void GameOver::initPos() {
    mPosX[GAMEOVER] = SCREEN_WIDTH / 2 - mTexture[GAMEOVER].getWidth() / 2;
    mPosY[GAMEOVER] = SCREEN_HEIGHT / 2 - mTexture[GAMEOVER].getHeight();

    mPosX[HOME] = SCREEN_WIDTH / 2 - mTexture[HOME].getWidth() - disBetweenHomeAndRetry;
    mPosY[HOME] = SCREEN_HEIGHT / 2 + mTexture[HOME].getHeight() / 3;

    mPosX[RETRY] = SCREEN_WIDTH / 2 + disBetweenHomeAndRetry;
    mPosY[RETRY] = SCREEN_HEIGHT / 2 + mTexture[RETRY].getHeight() / 3;

    mPosX[CONTINUE] = SCREEN_WIDTH / 2 - mTexture[CONTINUE].getWidth() / 2;
    mPosY[CONTINUE] = SCREEN_HEIGHT / 2 - mTexture[CONTINUE].getHeight();

    for (int i = 0; i < IMAGE_COUNT; i++) {
        addScale[i] = 0;
    }
}

void GameOver::loadIMG()
{
    //image: https://www.dreamstime.com/pixel-game-over-text-image-bit-assets-cross-stitch-pattern-t-shirt-design-vector-illustration-image222135423
    mTexture[GAMEOVER].loadFromFile(gameOverImagePath + "gameover.png");
    mTexture[HOME].loadFromFile(gameOverImagePath + "home.png");
    mTexture[RETRY].loadFromFile(gameOverImagePath + "retry.png");
    mTexture[CONTINUE].loadFromFile(gameOverImagePath + "continue.png");
}

void GameOver::render()
{
    //render game pause
    if (gamePaused) {
        mTexture[CONTINUE].render(mPosX[CONTINUE], mPosY[CONTINUE], NULL, 0, NULL, SDL_FLIP_NONE, addScale[CONTINUE]);
        mTexture[HOME].render(mPosX[HOME], mPosY[HOME], NULL, 0, NULL, SDL_FLIP_NONE, addScale[HOME]);
        mTexture[RETRY].render(mPosX[RETRY], mPosY[RETRY], NULL, 0, NULL, SDL_FLIP_NONE, addScale[RETRY]);
        return;
    }

    //render game over
    SDL_Rect clip;
    clip.x = 0; clip.y = 0;
    clip.w = loadGameOverState, clip.h = mTexture[GAMEOVER].getHeight();

    mTexture[GAMEOVER].render(mPosX[GAMEOVER], mPosY[GAMEOVER], &clip);

    //continue to load
    if (loadGameOverState + gameOverLoadSpeed <= mTexture[GAMEOVER].getWidth()) {
        loadGameOverState += gameOverLoadSpeed;
    }
    else {
        loadGameOverState = mTexture[GAMEOVER].getWidth();

        mTexture[HOME].render(mPosX[HOME], mPosY[HOME], NULL, 0, NULL, SDL_FLIP_NONE, addScale[HOME]);
        mTexture[RETRY].render(mPosX[RETRY], mPosY[RETRY], NULL, 0, NULL, SDL_FLIP_NONE, addScale[RETRY]);
    }
}

void GameOver::reset()
{
    loadGameOverState = 0;
    isOver = false;
}

BUTTON GameOver::handleEvent(SDL_Event *e)
{
    BUTTON buttonCollided = NONE;
    for (int button = HOMEBUTTON; button < BUTTON_COUNT; button++) {
        if (checkCollisionButton(static_cast<BUTTON>(button))) {
            buttonCollided = static_cast<BUTTON>(button);
        }
    }

    //finished loading game over image or game paused
    if (loadGameOverState == mTexture[GAMEOVER].getWidth() || gamePaused) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            if (e->button.button == SDL_BUTTON_LEFT) {
                // left mouse button pressed
                return buttonCollided;
            }
        }
    }

    return NONE;
}

bool GameOver::checkCollisionButton(BUTTON button)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (checkCollision(mouseX, mouseY, 0, 0, mPosX[button], mPosY[button], mTexture[button].getWidth(), mTexture[button].getHeight())) {
        addScale[button] = addScaleButton;

        return true;
    }
    else {
        //reset;
        addScale[button] = 0;

        return false;
    }
}

#endif //GAMEOVER_H
