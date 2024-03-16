#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "../GunAndBullet/Guns.h"
#include "../Scene/Pipe.h"
#include "../Game/GameOver.h"
#include "Character.h"

const int mainBirdPosX = 200;
const int mainBirdPosY = SCREEN_HEIGHT / 2;

class MainBird : public Character
{
private:
    Guns gun;

public:
    MainBird(int x, int y);
    ~MainBird();

    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    bool render() override;

    ObjectsList &getBulletList();
};

MainBird::MainBird(int x, int y) : Character(x, y, MAIN_BIRD)
{
    init(x, y);
}

MainBird::~MainBird()
= default;

void MainBird::init(int x, int y)
{
    initCharacter(x, y);
    gun.setPosAndAngle(x + getWidth() / 3, y + getHeight() / 3);
    gun.init();
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
                setVelY(15);
                setVelAngle(8);
                break;
            case SDLK_w:
                setVelY(15);
                setVelAngle(8);
                break;

            default:
                break;
        }
    }
    else {
        gun.handleEvent(e);
    }
}

void MainBird::handleKey(const Uint8 *currentKeyStates)
{
    if (GameOver::gameIsOver()) {
        return;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        setVelX(-5);
    }
    else if (currentKeyStates[SDL_SCANCODE_D]) {
        setVelX(5);
    }
    else setVelX(0);
}

bool MainBird::render()
{
    bool rendered = renderCharacter();

    if (!GameOver::gameIsOver()) {
        gun.setPosAndAngle(mPosX + getWidth() / 3, mPosY + getHeight() / 3);
        gun.render();
    }
    else {
        onDied();
    }

    if (!isDied()) {
        decreaseVelAndAngle();
    }

    return rendered;
}

ObjectsList &MainBird::getBulletList()
{
    return gun.getBulletList();
}

#endif //MAINBIRD_H
