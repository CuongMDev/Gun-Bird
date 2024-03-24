#ifndef BOSS_H
#define BOSS_H

#include "Character.h"
#include "../Other/ObjectsList.h"
#include "HealthBar.h"

class Boss : public Character
{
private:
    const int dragonSpeed = 1;
    const int dragonTimeBeforeBeingDeleted = 2000;
    static Uint32 nextCreatedTime;

    HealthBar *health;
    int downTime;

    bool checkDownTime();

public:
    Boss();
    ~Boss();

    void init();

    void decreaseHealth(int value);
    bool render() override;
};

Uint32 Boss::nextCreatedTime = waitTimeBeforePlaying;

Boss::Boss() : Character(0, 0, BOSS)
{
    init();
}

Boss::~Boss()
= default;

void Boss::init()
{
    downTime = -1;
    initCharacter(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - getHeight() / 2);
    health = new HealthBar(0, 0, true, 100);
}

bool Boss::render()
{
    bool rendered = renderCharacter();
    if (!rendered) {
        //check downtime before death
        rendered = !checkDownTime();
    }

    if (mPosX <= 0) {
        rendered = false;
    }
    if (!isDied()) {
        health->updatePos(mPosX + getWidth() / 2, mPosY - 10);
        health->render();
        //go in screen
        if (mPosX + getWidth() > SCREEN_WIDTH - 50) {
            mPosX -= dragonSpeed;
        }
    }

    return rendered;
}

bool Boss::checkDownTime()
{
    if (downTime == -1) {
        downTime = SDL_GetTicks() + dragonTimeBeforeBeingDeleted;
        return false;
    }

    if (SDL_GetTicks() >= downTime) {
        return true;
    }
    return false;
}

void Boss::decreaseHealth(int value)
{
    health->changeHealth(-value);
    if (health->getCurrentHealth() == 0) {
        onDied();
        return;
    }
}

#endif //BOSS_H
