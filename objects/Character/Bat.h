#ifndef BAT_H
#define BAT_H

#include "Character.h"
#include "../Other/ObjectsList.h"

class Bat : public Character
{
private:
    static const int batPosX = SCREEN_WIDTH;
    const int batSpeed = 1;
    const int batTimeBeforeBeingDeleted = 1000;
    //random time interval after nextCreatedTime
    static const std::pair<int, int> randomTimeInterval;
    static const std::pair<int, int> randomHeightInterval;
    //pipe will be created when current time = this
    static Uint32 nextCreatedTime;

    static bool onAdd;
    HealthBar *health;
    int downTime;

    bool checkDownTime();

public:
    Bat(int x, int y);
    ~Bat();

    static void spawnBat(ObjectsList *batList);
    static void renderAll(ObjectsList *batList);
    static void resetTime();

    void init(int x, int y);

    void decreaseHealth(int value);
    bool render() override;
};

const std::pair<int, int> Bat::randomTimeInterval = {500, 1000};
const std::pair<int, int> Bat::randomHeightInterval = {50, groundPosY - 50};
Uint32 Bat::nextCreatedTime = waitTimeBeforePlaying;

Bat::Bat(int x, int y) : Character(x, y, BAT)
{
    init(x, y);
}

Bat::~Bat()
= default;

void Bat::init(int x, int y)
{
    downTime = -1;
    initCharacter(x, y);
    setVelX(-10);
    health = new HealthBar(0, 0, true, 12);
}

bool Bat::render()
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
//        if (mPosX + getWidth() > SCREEN_WIDTH - 50) {
//            mPosX -= batSpeed;
//        }
    }

    return rendered;
}

bool Bat::onAdd = false;

bool Bat::checkDownTime()
{
    if (downTime == -1) {
        downTime = SDL_GetTicks() + batTimeBeforeBeingDeleted;
        return false;
    }

    if (SDL_GetTicks() >= downTime) {
        return true;
    }
    return false;
}

void Bat::spawnBat(ObjectsList *batList)
{
    if (SDL_GetTicks() >= nextCreatedTime) {
        //bat Y
        int batPosY = getRandomNumber(randomHeightInterval.first, randomHeightInterval.second);
        //create new bat
        Bat *bat = new Bat(batPosX, batPosY);
        batList->add(bat);

        //reset created time
        nextCreatedTime = SDL_GetTicks() + getRandomNumber(randomTimeInterval.first, randomTimeInterval.second);
    }
}

void Bat::decreaseHealth(int value)
{
    health->changeHealth(-value);
    if (health->getCurrentHealth() == 0) {
        onDied();
        return;
    }
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
    nextCreatedTime = SDL_GetTicks() + waitTimeBeforePlaying;
}

#endif //BAT_H
