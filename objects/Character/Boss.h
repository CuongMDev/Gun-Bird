#ifndef BOSS_H
#define BOSS_H

#include "Character.h"
#include "../Other/ObjectsList.h"
#include "HealthBar.h"
#include "../GunAndBullet/Bullets.h"

const int bossHealthBarPosX = SCREEN_WIDTH - 200;
const int bossHealthBarPosY = 50;

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

    enum BOSS_SOUND_TYPE
    {
        WING_FLAP_SOUND,
        SYRINGE_SOUND,

        BOSS_SOUND_COUNT
    };

    const int bossDamage = 1;
    const int bossTimeBeforeBeingDeleted = 2000;
    //-1 if it is having action
    static Uint32 nextActionTime;

    int maxHealth;
    Mix_Chunk* bossSound[BOSS_SOUND_COUNT];

    const int attackPosX = SCREEN_WIDTH - 300;
    const int attackPosY = 50;

    const int movePosX = SCREEN_WIDTH - 200;
    const int movePosY = groundPosY / 2;

    static const int imgCount = 8;
    const int initMaxHealth = 100;
    const int healthAddPerRound = 30;

    const int randomActionTime = 1000;

    std::vector<LTexture> sTexture[ACTION_COUNT];
    std::vector<LTexture> dieTexture;

    Action currentAction;

    HealthBar *health;
    int downTime;

    ObjectsList syringeList;

    bool actionStarted;

    int attackActionCount;
    int curAttackActionTime;

    int bossSpeed;
    bool hasBeenDamaged;

    const int shootDelay = 13;
    //shoot when curShootTime=shootDelay
    int curShootTime;

    bool checkDownTime();

    void setAction(const Action &action);

    //true if moved to x, y, if getThisVel = true, the vel in moveTo() will be set for this class
    bool moveTo(int x, int y, bool getThisVel = false);
    bool moveAction();

    bool attackAction();
    bool moveAttackAction();
    void randomMoveAttackDes(int &desX, int &desY);

    void activeWingFlapSound();
    void activeSyringeSound();

    //false if no action to do
    bool startAction();
    void addSyringe();
    void setAngle(double angle) override;

    void onDied() override;

    void randomAction();
    void checkRandomTime();
    void reset();

    bool addAttackAction();

    void loadIMG();
    void loadSound();

    bool updateState() override;
    void renderTogRenderer() override;

public:
    Boss();
    ~Boss();

    void init();
    void continueInit();

    ObjectsList &getSyringeList();

    void decreaseHealth(int value);
};

Uint32 Boss::nextActionTime = waitTimeBeforePlaying;

Boss::Boss() : Character(0, 0, BOSS)
{
    loadIMG();
    loadSound();
    health = new HealthBar(bossHealthBarPosX, bossHealthBarPosY, true, true, 0);
}

Boss::~Boss()
{
    for (auto & sound : bossSound) {
        Mix_FreeChunk(sound);
    }
}

void Boss::loadIMG()
{
    //image: https://vi.cleanpng.com/png-hrcuaj/
    for (auto & actionTexture : sTexture) {
        actionTexture.resize(imgCount);
    }
    for (int i = 0; i < imgCount; i++) {
        sTexture[MOVE][i].loadFromFile(bossImagePath + "Move/move" + std::to_string(i) + ".png", true, 34,177,76);
        sTexture[ATTACK][i].loadFromFile(bossImagePath + "Attack/attack" + std::to_string(i) + ".png", true, 34,177,76);
        sTexture[MOVE_ATTACK][i].loadFromFile(bossImagePath + "AttackMove/attackmove" + std::to_string(i) + ".png", true, 34,177,76);
    }

    dieTexture.resize(1);
    dieTexture[0].loadFromFile(bossImagePath + "Die/die0" + ".png", true, 34,177,76);
}

void Boss::loadSound()
{
    bossSound[WING_FLAP_SOUND] = Mix_LoadWAV((bossSoundPath + "wingflap.wav").c_str());
    bossSound[SYRINGE_SOUND] = Mix_LoadWAV((bossSoundPath + "syringe.wav").c_str());
}

void Boss::init()
{
    attackActionCount = 0;
    curAttackActionTime = -1;
    maxHealth = initMaxHealth - healthAddPerRound;

    //set not appear
    setAction(MOVE);
    onDied();
    downTime = 0;
}

void Boss::continueInit()
{
    attackActionCount++;

    downTime = -1;
    maxHealth += healthAddPerRound;
    health->setMaxHealth(maxHealth);
    health->setHealth(maxHealth);

    hasBeenDamaged = false;
    syringeList.reset();

    setAction(MOVE);
    reset();
    initCharacter(SCREEN_WIDTH + getHeight(), getRandomNumber(0, groundPosY - getHeight()));

    setGravity(false);
    setBorder(-getWidth(), -getHeight(), SCREEN_WIDTH + 2 * getWidth(), SCREEN_HEIGHT + 2 * getHeight());
}

void Boss::reset()
{
    curShootTime = 0;
    nextActionTime = -1;
    bossSpeed = 10;

    setVelX(0);
    setVelY(0);

    Character::setAngle(0);
    setFlipMode(SDL_FLIP_NONE);
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
    if (isDied() && checkDownTime()) {
        return false;
    }

    bool updated = Character::updateState();
    if (!updated) {
        //check downtime before death
        updated = !checkDownTime();
    }
    if (mPosX <= 0) {
        updated = false;
    }

    if (hasBeenDamaged) {
        setAlpha(100);
        hasBeenDamaged = false;
    }
    else setAlpha(255);

    if (!isDied()) {
        if (!startAction()) {
            if (!GameOver::gameIsOver() || currentAction != MOVE) {
                checkRandomTime();
            }
        }
        if (getCurrentIMG() == 3 && checkEndOfIMG()) {
            activeWingFlapSound();
        }
    }

    return updated;
}

void Boss::renderTogRenderer()
{
    if (isDied() && checkDownTime()) {
        return;
    }
    health->render();

    syringeList.renderAll();
    Character::renderTogRenderer();
}

void Boss::decreaseHealth(int value)
{
    health->changeHealth(-value);
    if (health->getCurrentHealth() == 0) {
        onDied();
        return;
    }

    hasBeenDamaged = true;
}

void Boss::setAction(const Action &action)
{
    actionStarted = false;
    setSTexture(&sTexture[MOVE]);
    currentAction = action;
}

bool Boss::moveTo(int x, int y, bool getThisVel)
{
    if (getCurrentIMG() == 4 && checkEndOfIMG()) {
        activeWingFlapSound();
    }
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

bool Boss::startAction()
{
    switch (currentAction) {
        case MOVE:
            return moveAction();
            break;
        case ATTACK:
            return attackAction();
            break;
        case MOVE_ATTACK:
            return moveAttackAction();
            break;

        default:
            return false;
            break;
    }
}

bool Boss::moveAction()
{
    if (!actionStarted) {
        if (moveTo(movePosX, movePosY)) {
            actionStarted = true;
        }

        return true;
    }

    return false;
}

bool Boss::attackAction()
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
        if (currentIMG == 4 && checkEndOfIMG()) {
            activeWingFlapSound();
        }
        if (currentIMG == 7 && checkEndOfIMG()) { //end of chain
            //reset
            if (addAttackAction()) {
                return true;
            }

            reset();
            return false;
        }
    }

    return true;
}

bool Boss::moveAttackAction()
{
    //des will go to
    static int desX, desY = SCREEN_HEIGHT - getHeight();

    if (!actionStarted) {
        //move to out the border before start action
        if (moveTo(mPosX, -getHeight())) {
            setSTexture(&sTexture[MOVE_ATTACK]);
            actionStarted = true;

            addAttackAction();
            randomMoveAttackDes(desX, desY);
        }
    }

    if (actionStarted) {
        //is in screen
        if (mPosY > SCREEN_HEIGHT) {
            if (addAttackAction()) {
                randomMoveAttackDes(desX, desY);
                return true;
            }

            //reset
            reset();
            return false;
        }
    }

    return true;
}

void Boss::randomAction()
{
    Action randomAct = static_cast<Action>(getRandomNumber(ATTACK, ACTION_COUNT - 1));
    setAction(randomAct);
}

void Boss::checkRandomTime()
{
    if (currentAction != MOVE) {
        setAction(MOVE);
        return;
    }

    if (nextActionTime == -1) {
        nextActionTime = getCurrentTime() + randomActionTime;
    }
    else if (getCurrentTime() >= nextActionTime) {
        randomAction();
    }
}

void Boss::addSyringe()
{
    curShootTime++;
    if (curShootTime >= shootDelay) {
        activeSyringeSound();
        syringeList.add(new Bullets(getRandomNumber(0, SCREEN_WIDTH), 0, SYRINGE_BULLET, bossDamage, getRandomNumber(0, SCREEN_WIDTH), SCREEN_HEIGHT));
        curShootTime = 0;
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

bool Boss::addAttackAction()
{
    curAttackActionTime++;
    if (curAttackActionTime < attackActionCount) {
        return true;
    }

    curAttackActionTime = -1;
    return false;
}

void Boss::randomMoveAttackDes(int &desX, int &desY)
{
    bossSpeed = 25;

    teleportToPosX(getRandomNumber(0, SCREEN_WIDTH));
    teleportToPosY(-getHeight());
    desX = getRandomNumber(0, SCREEN_WIDTH);
    desY = SCREEN_HEIGHT - getHeight();

    double angle = angleBetweenTwoPos(mPosX, mPosY, desX, desY);
    setAngle(angle);

    moveTo(desX, desY, true);
}

void Boss::activeWingFlapSound()
{
    static int currentChannel = 0;

    currentChannel++;
    if (currentChannel >= 3) {
        currentChannel = 0;
    }
    Mix_PlayChannel(currentChannel + (int)BOSS_SOUND_CHANNEL::WING_FLAP_1, bossSound[WING_FLAP_SOUND], 0);
}

void Boss::activeSyringeSound() {
    static int currentChannel = 0;

    currentChannel++;
    if (currentChannel >= 2) {
        currentChannel = 0;
    }
    Mix_PlayChannel(currentChannel + (int)BOSS_SOUND_CHANNEL::SYRINGE_1, bossSound[SYRINGE_SOUND], 0);
}

ObjectsList &Boss::getSyringeList()
{
    return syringeList;
}

void Boss::onDied()
{
    setSTexture(&dieTexture);
    Character::onDied();
}

#endif //BOSS_H
