#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "Guns.h"
#include "Pipe.h"
#include "GameOver.h"
#include "Character.h"

class MainBird :private Guns
{
private:
    Character *mCharacter;
    bool checkPipeCollision();

public:
    MainBird(int x, int y);
    ~MainBird();

    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);

    bool render();
};

MainBird::MainBird(int x, int y)
{
    mCharacter = new Character(x, y, MAIN_BIRD);
    init(x, y);
}

MainBird::~MainBird()
= default;

void MainBird::init(int x, int y)
{
    mCharacter->init(x, y);
    setGunPosAndAngle(x + mCharacter->getWidth() / 3, y + mCharacter->getHeight() / 3);
    initGun();
}

bool MainBird::checkPipeCollision()
{
    for (auto pipe : pipeList) {
        if (checkCollision(pipe->getPosX(), pipe->getPosY(), pipe->getWidth(), pipe->getHeight(),
                           mCharacter->getPosX(), mCharacter->getPosY(), mCharacter->getWidth(), mCharacter->getHeight())) {

            return true;
        }
    }

    return false;
}

void MainBird::handleEvent(SDL_Event *e)
{
    if (GameOver::gameIsOver()) {
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch (e->key.keysym.sym) {
            case SDLK_SPACE:
                mCharacter->setVelY(15);
                mCharacter->setVelAngle(8);
                break;

            default:
                break;
        }
    }
    else {
        handleGunEvent(e);
    }
}

void MainBird::handleKey(const Uint8 *currentKeyStates)
{
    if (GameOver::gameIsOver()) {
        return;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        mCharacter->setVelX(-5);
    }
    else if (currentKeyStates[SDL_SCANCODE_D]) {
        mCharacter->setVelX(5);
    }
    else mCharacter->setVelX(0);
}

bool MainBird::render()
{
    bool rendered = mCharacter->render();

    if (!GameOver::gameIsOver()) {
        setGunPosAndAngle(mCharacter->getPosX() + mCharacter->getWidth() / 3, mCharacter->getPosY() + mCharacter->getHeight() / 3);
        renderGun();
        if (checkPipeCollision()) {
            GameOver::onGameOver();
        }
    }
    else {
        mCharacter->onDied();
    }

    if (!mCharacter->isDied()) {
        mCharacter->decreaseVelAndAngle();
    }

    return rendered;
}

#endif //MAINBIRD_H
