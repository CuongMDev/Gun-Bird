#ifndef BOSS_H
#define BOSS_H

#include "Character.h"
#include "../Other/ObjectsList.h"
#include "HealthBar.h"
#include "../GunAndBullet/Bullets.h"

class Boss : public Character
{
private:
    enum Action
    {
        MOVE,
        ATTACK,
        MOVE_ATTACK,

        ACTION_COUNT
    };

    const int bossDamage = 1;
    const int bossTimeBeforeBeingDeleted = 2000;
    static Uint32 nextCreatedTime;

    const int attackPosX = SCREEN_WIDTH - 300;
    const int attackPosY = 50;

    static const int imgCount = 8;

    std::vector<LTexture> sTexture[ACTION_COUNT];

    Action currentAction;

    HealthBar *health;
    int downTime;

    ObjectsList sryngeList;

    bool actionStarted;

    int bossSpeed;

    const int shootDelay = 13;
    //shoot when curTime=shootDelay
    int curTime;

    bool checkDownTime();

    void setAction(const Action &action);

    //true if moved to x, y, if getThisVel = true, the vel in moveTo() will be set for this class
    bool moveTo(int x, int y, bool getThisVel = false);
    void moveAction();
    void attackAction();
    void moveAttackAction();
    void startAction();
    void addSyringe();
    void setAngle(double angle);

    bool updateState() override;
    void renderTogRenderer() override;

public:
    Boss();
    ~Boss();

    void init();
    void loadIMG();

    void decreaseHealth(int value);
};

Uint32 Boss::nextCreatedTime = waitTimeBeforePlaying;

Boss::Boss() : Character(0, 0, BOSS)
{
    loadIMG();
    init();
    setBorder(0, -getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT + 2 * getHeight());
}

Boss::~Boss()
= default;

void Boss::loadIMG()
{
    for (int action = 0; action < ACTION_COUNT; action++) {
        sTexture[action].resize(imgCount);
    }
    for (int i = 0; i < imgCount; i++) {
        sTexture[MOVE][i].loadFromFile(bossImagePath + "Move/move" + std::to_string(i) + ".png", true, 34,177,76);
        sTexture[ATTACK][i].loadFromFile(bossImagePath + "Attack/attack" + std::to_string(i) + ".png", true, 34,177,76);
        sTexture[MOVE_ATTACK][i].loadFromFile(bossImagePath + "AttackMove/attackmove" + std::to_string(i) + ".png", true, 34,177,76);
    }
}

void Boss::init()
{
    initCharacter(SCREEN_WIDTH, 0);
    curTime = 0;
    downTime = -1;
    bossSpeed = 10;
    setGravity(false);
    health = new HealthBar(0, 0, true, 100);

    actionStarted = false;
    setAction(MOVE_ATTACK);
    setSTexture(&sTexture[MOVE]);
    sryngeList.reset();
}

bool Boss::checkDownTime()
{
    if (downTime == -1) {
        downTime = getCurrentTime() + bossTimeBeforeBeingDeleted;
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
//        if (mPosX + getWidth() > SCREEN_WIDTH - 50) {
//            mPosX -= bossSpeed;
//        }

        startAction();
    }

    return updated;
}

void Boss::renderTogRenderer()
{
    if (!isDied()) {
        health->render();
    }

    sryngeList.renderAll();
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

void Boss::setAction(const Action &action)
{
    currentAction = action;
}

bool Boss::moveTo(int x, int y, bool getThisVel)
{
    if (mPosX == x && mPosY == y) {
        return true;
    }

    int velX = mPosX, velY = mPosY;
    calculateVelocityBetweenTwoPos(velX, velY, x, y, bossSpeed);
    if (getThisVel) {
        setVelX(velX);
        setVelY(velY);
    }

    if (abs(mPosX - x) < abs(velX)) mPosX = x;
    else mPosX += velX;
    if (abs(mPosY - y) < abs(velY)) mPosY = y;
    else mPosY += velY;

    if (mPosX == x && mPosY == y) {
        return true;
    }
    return false;
}

void Boss::startAction()
{
    switch (currentAction) {
        case MOVE:
            moveAction();
            break;
        case ATTACK:
            attackAction();
            break;
        case MOVE_ATTACK:
            moveAttackAction();
            break;

        default:
            break;
    }
}

void Boss::moveAction()
{
    setSTexture(&sTexture[MOVE]);
}

void Boss::attackAction()
{
    if (!actionStarted) {
        //move to pos before start action
        if (moveTo(attackPosX, attackPosY)) {
            setSTexture(&sTexture[ATTACK]);
            actionStarted = true;
        }
    }

    if (actionStarted) {
        int currentIMG = getCurrentIMG();
        if (3 <= currentIMG && currentIMG <= 7) {
            //shoot
            addSyringe();
        }
        if (currentIMG == 7) { //end of chain
            //reset
            curTime = 0;
        }
    }
}

void Boss::moveAttackAction()
{
    //des will go to
    static int desX, desY = SCREEN_HEIGHT - getHeight();

    if (!actionStarted) {
        //move to out the border before start action
        if (moveTo(mPosX, -getHeight())) {
            setSTexture(&sTexture[MOVE_ATTACK]);
            actionStarted = true;
            bossSpeed = 20;

            mPosX = getRandomNumber(0, SCREEN_WIDTH);
            desX = getRandomNumber(0, SCREEN_WIDTH);
            double angle = angleBetweenTwoPos(mPosX, mPosY, desX, desY);
            setAngle(angle);

            moveTo(desX, desY, true);
        }
    }

    if (actionStarted) {
        //move until out the border
        if (mPosY > SCREEN_WIDTH) {
            //reset
            bossSpeed = 10;
//            setAngle(0);
        }
        else {
        }
    }
}

void Boss::addSyringe()
{
    curTime++;
    if (curTime >= shootDelay) {
        sryngeList.add(new Bullets(getRandomNumber(0, SCREEN_WIDTH), 0, SYRINGE_BULLET, bossDamage, getRandomNumber(0, SCREEN_WIDTH), SCREEN_HEIGHT));
        curTime = 0;
    }
}

void Boss::setAngle(double angle)
{
    if (angle < 90) {
        setFlipMode(SDL_FLIP_HORIZONTAL);
        angle -= 30;
    }
    else {
        angle -= 150;
        setFlipMode(SDL_FLIP_NONE);
    }
    Character::setAngle(angle);

}

#endif //BOSS_H
