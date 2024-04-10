#ifndef BAT_H
#define BAT_H

#include "Character.h"
#include "../Other/ObjectsList.h"

const int batDamage = 1;

class Bat : public Character
{
private:
    enum BAT_TYPE
    {
        BAT_RED_TYPE,
        BAT_PURPLE_TYPE,

        BAT_TYPE_COUNT
    };

    static const int batPosX = SCREEN_WIDTH - 1;
    const int batTimeBeforeBeingDeleted = 1000;
    //random time interval after nextCreatedTime
    static const std::pair<int, int> randomTimeInterval;
    static const std::pair<int, int> randomHeightInterval;
    //bat will be created when current time = this
    static Uint32 nextCreatedTime;

    static std::vector<LTexture> sTexture[BAT_TYPE_COUNT];

    BAT_TYPE currentType;

    HealthBar *health;
    int downTime;
    bool hasBeenDamaged;
    bool typeChanged;

    bool checkDownTime();

    static void loadIMG();
    void setAngle(double angle) override;

    void setVelAndAngle(int x, int y, double speed);
    void changeType(bool isDamaged = false);

    int getRedBatSpeed();
    int getPurpleBatSpeed();

    bool updateState() override;
    void renderTogRenderer() override;

public:
    Bat(int x, int y, BAT_TYPE batType);
    ~Bat();

    static void spawnBat(ObjectsList *batList);
    static void renderAll(ObjectsList *batList);
    static void resetTime();

    void init(int x, int y, BAT_TYPE batType);

    void decreaseHealth(int value);
};

const std::pair<int, int> Bat::randomTimeInterval = {500, 700};
const std::pair<int, int> Bat::randomHeightInterval = {50, groundPosY - 50};
Uint32 Bat::nextCreatedTime = waitTimeBeforePlaying;

Bat::Bat(int x, int y, BAT_TYPE batType) : Character(x, y, BAT)
{
    loadIMG();
    init(x, y, batType);

    health = new HealthBar(0, 0, true, false, 12);
}

Bat::~Bat()
{
    delete health;
}

void Bat::init(int x, int y, BAT_TYPE batType)
{
    downTime = -1;
    setSTexture(&sTexture[batType]);
    initCharacter(x, y);

    currentType = batType;
    if (batType == BAT_RED_TYPE) {
        setVelX(-getRedBatSpeed());
        typeChanged = false;
    }
    else {
        setVelAndAngle(gMainBirdPosX, gMainBirdPosY, getPurpleBatSpeed()); //purple
        typeChanged = true;
    }

    setGravity(false);
    setBorder(-2 * getWidth(), -2 * getHeight(), SCREEN_WIDTH + 4 * getWidth(), SCREEN_HEIGHT + 4 * getHeight());
}

std::vector<LTexture> Bat::sTexture[BAT_TYPE_COUNT] = {};

void Bat::loadIMG()
{
    static const int imgCount = 4;
    static bool loadedIMG = false;
    if (loadedIMG) {
        return;
    }
    loadedIMG = true;

    //image: https://opengameart.org/content/bat-rework
    for (auto & batTexture : sTexture) {
        batTexture.resize(imgCount);
    }
    for (int i = 0; i < imgCount - 1; i++) {
        sTexture[BAT_RED_TYPE][i].loadFromFile(batImagePath + "bat" + std::to_string(i) + ".png");
        sTexture[BAT_PURPLE_TYPE][i].loadFromFile(batImagePath + "purplebat" + std::to_string(i) + ".png");
    }
    sTexture[BAT_RED_TYPE][imgCount - 1] = sTexture[BAT_RED_TYPE][1];
    sTexture[BAT_PURPLE_TYPE][imgCount - 1] = sTexture[BAT_PURPLE_TYPE][1];
}

void Bat::setVelAndAngle(int x, int y, double speed)
{
    int velX = mPosX, velY = mPosY;
    calculateVelocityBetweenTwoPos(velX, velY, x, y, speed);
    setAngle(angleBetweenTwoPos(mPosX, mPosY, x, y) + 180 * (speed < 0));

    setVelX(velX);
    setVelY(velY);
}

bool Bat::updateState()
{
    bool updated = Character::updateState();
    if (!updated) {
        //check downtime before death
        updated = !checkDownTime();
    }
    if (!checkCollision(mPosX, mPosY, getWidth(), getHeight(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        updated = false;
    }

    if (hasBeenDamaged) {
        setAlpha(100);
        hasBeenDamaged = false;
    }
    else setAlpha(255);

    if (!isDied()) {
        health->updatePos(mPosX + getWidth() / 2, mPosY - 10);
        changeType();
    }

    return updated;
}

void Bat::renderTogRenderer()
{
    if (!isDied()) {
        health->render();
    }

    Character::renderTogRenderer();
}

bool Bat::checkDownTime()
{
    if (downTime == -1) {
        downTime = getCurrentTime() + batTimeBeforeBeingDeleted;
        return false;
    }

    if (getCurrentTime() >= downTime) {
        return true;
    }
    return false;
}

void Bat::spawnBat(ObjectsList *batList)
{
    if (getCurrentTime() >= nextCreatedTime) {
        //bat Y
        int batPosY = getRandomNumber(randomHeightInterval.first, randomHeightInterval.second);
        int batType = getRandomWithPercent({90, 10}, std::vector<BAT_TYPE>{BAT_RED_TYPE, BAT_PURPLE_TYPE});
        //create new bat
        Bat *bat = new Bat(batPosX, batPosY, static_cast<BAT_TYPE>(batType));
        batList->add(bat);

        //reset created time
        nextCreatedTime = getCurrentTime() + getRandomNumber(randomTimeInterval.first, randomTimeInterval.second);
    }
}

void Bat::decreaseHealth(int value)
{
    health->changeHealth(-value);
    if (health->getCurrentHealth() == 0) {
        onDied();
        return;
    }
    changeType(true);

    hasBeenDamaged = true;
}

void Bat::renderAll(ObjectsList *batList)
{
    if (!GameOver::gameIsOver()) {
        spawnBat(batList);
    }

    batList->renderAll();
}

void Bat::resetTime()
{
    nextCreatedTime = getCurrentTime() + waitTimeBeforePlaying;
}

void Bat::setAngle(double angle)
{
    while (angle >= 180) {
        angle -= 360;
    }
    if (abs(angle) < 90) {
        setFlipMode(SDL_FLIP_HORIZONTAL);
        angle += 10;
    }
    else {
        angle += 160;
    }

    while (angle >= 180) {
        angle -= 360;
    }
    Character::setAngle(angle);
}

void Bat::changeType(bool isDamaged)
{
    if (typeChanged) {
        if (isDamaged && currentType == BAT_PURPLE_TYPE) {
            setVelAndAngle(gMainBirdPosX, gMainBirdPosY, getPurpleBatSpeed());
        }
        return;
    }

    if (isDamaged) {
        bool willChange = getRandomWithPercent({60, 40}, std::vector<bool>{false, true});
        if (willChange) {
            init(mPosX, mPosY, BAT_PURPLE_TYPE);
        }
        else { //run
            setVelAndAngle(gMainBirdPosX, gMainBirdPosY, -getRedBatSpeed());
            typeChanged = true;
        }
    }

    //-------------------

    bool willChange = getRandomWithPercent({199, 1}, std::vector<bool>{false, true});
    if (willChange) {
        init(mPosX, mPosY, BAT_PURPLE_TYPE);
    }
}

int Bat::getRedBatSpeed()
{
    return gVelocityYScene + 2;
}

int Bat::getPurpleBatSpeed()
{
    return getRedBatSpeed() + 5;
}

#endif //BAT_H
