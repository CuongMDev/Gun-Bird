#ifndef MAINBIRD_H
#define MAINBIRD_H

#include "../GunAndBullet/Gun.h"
#include "../Scene/Pipe.h"
#include "../Game/GameOver.h"
#include "Character.h"
#include "HealthBar.h"

const int mainBirdPosX = 200;
const int mainBirdPosY = SCREEN_HEIGHT / 2;

const int mainHealthBarPosX = 10;
const int mainHealthBarPosY = 50;

class MainBird : public Character
{
private:
    const int maxHealth = 5;
    const int invisibleTime = 2000;

    Gun gun;
    HealthBar *health;

    //-1 if not invisible
    int invisibleEndTime;
    void setInvisible(bool state);
    bool checkVisible();
    void updateVisibleState();
    void jump();

    const int maxVisibleState = 6;
    //blurry if <= maxVisibleState / 2 and clear if > maxVisibleState / 2
    int currentVisibleState;

    bool updateState() override;
    void renderTogRenderer() override;

public:
    MainBird(int x, int y);
    ~MainBird();

    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    void changeHealth(int value);
    void addBulletCount(GUN_TYPE gunType, int bulletCount);

    int getCurrentHealth() const;

    ObjectsList &getBulletList();
};

MainBird::MainBird(int x, int y) : Character(x, y, MAIN_BIRD)
{
    health = new HealthBar(mainHealthBarPosX, mainHealthBarPosY, false, true, maxHealth);
    init(x, y);
}

MainBird::~MainBird()
{
    delete health;
}

void MainBird::init(int x, int y)
{
    initCharacter(x, y);
    health->setHealth(maxHealth);
    gun.setPosAndAngle(x + getWidth() / 3, y + getHeight() / 3);
    gun.init();

    invisibleEndTime = -1;
    currentVisibleState = 0;
}

void MainBird::handleEvent(SDL_Event *e)
{
    if (GameOver::gameIsOver()) {
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch (e->key.keysym.sym) {

            default:
                break;
        }
    }

    gun.handleEvent(e);
}

void MainBird::handleKey(const Uint8 *currentKeyStates)
{
    if (GameOver::gameIsOver()) {
        return;
    }

    static int spaceRepeat = 0;
    if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        if (!spaceRepeat || spaceRepeat > 30) {
            jump();
        }

        spaceRepeat++;
    }
    else spaceRepeat = 0;

    setVelX(0);
    if (currentKeyStates[SDL_SCANCODE_D]) {
        setVelX(5);
    }
    if (currentKeyStates[SDL_SCANCODE_A]) {
        setVelX(-5);
    }
}

void MainBird::setInvisible(bool state)
{
    if (state) {
        invisibleEndTime = getCurrentTime() + invisibleTime;
    }
    else {
        invisibleEndTime = -1;
        setAlpha(255);
    }
}

bool MainBird::checkVisible()
{
    if (invisibleEndTime == -1) {
        return false;
    }
    if (getCurrentTime() < invisibleEndTime) {
        return true;
    }
    else {
        setInvisible(false);
        return false;
    }
}

void MainBird::updateVisibleState()
{
    if (currentVisibleState <= maxVisibleState / 2) {
        mTexture->setAlpha(100);
    }
    else {
        mTexture->setAlpha(255);
    }

    currentVisibleState++;
    if (currentVisibleState >= maxVisibleState) {
        currentVisibleState = 0;
    }
}

void MainBird::jump()
{
    setVelY(-15);
    setVelAngle(8);
}

bool MainBird::updateState() {
    bool updated = Character::updateState();

    if (checkVisible()) {
        updateVisibleState();
    }
    if (!GameOver::gameIsOver()) {
        gun.setPosAndAngle(mPosX + getWidth() / 3, mPosY + getHeight() / 3);
    }

    if (!isDied()) {
        decreaseVelAndAngle();
    }

    return updated;
}

void MainBird::renderTogRenderer()
{
    Character::renderTogRenderer();
    if (!GameOver::gameIsOver()) {
        gun.render();
    }
    health->render();
}

void MainBird::changeHealth(int value)
{
    //if get damaged && visible
    if (value < 0 && checkVisible()) {
        return;
    }
    health->changeHealth(value);
    if (health->getCurrentHealth() == 0) {
        onDied();
        setInvisible(false);
        return;
    }

    //if get damaged
    if (value < 0) {
        setInvisible(true);
    }
}

void MainBird::addBulletCount(GUN_TYPE gunType, int bulletCount)
{
    gun.addBulletCount(gunType, bulletCount);
}

int MainBird::getCurrentHealth() const
{
    return health->getCurrentHealth();
}

ObjectsList &MainBird::getBulletList()
{
    return gun.getBulletList();
}

#endif //MAINBIRD_H
