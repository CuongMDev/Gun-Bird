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

    bool updateState() override;
    void renderTogRenderer() override;

public:
    Boss();
    ~Boss();

    void init();

    void decreaseHealth(int value);
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
    initCharacter(SCREEN_WIDTH, 0);
//    setVelX(-10);
//    setVelY(-10);
    health = new HealthBar(0, 0, true, 100);
}

bool Boss::checkDownTime()
{
    if (downTime == -1) {
        downTime = getCurrentTime() + dragonTimeBeforeBeingDeleted;
        return false;
    }

    if (getCurrentTime() >= downTime) {
        return true;
    }
    return false;
}

bool Boss::updateState()
{
    bool updated = Character::updateState();
    if (!updated) {
        //check downtime before death
        updated = !checkDownTime();
    }
    if (mPosX <= 0) {
        updated = false;
    }

    if (!isDied()) {
        health->updatePos(mPosX + getWidth() / 2, mPosY - 10);
        //go in screen
        if (mPosX + getWidth() > SCREEN_WIDTH - 50) {
            mPosX -= dragonSpeed;
        }
    }

    return updated;
}

void Boss::renderTogRenderer()
{
    if (!isDied()) {
        health->render();
    }

    Character::renderTogRenderer();
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
