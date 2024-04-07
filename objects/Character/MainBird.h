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
    enum BIRD_SOUND_TYPE
    {
        JUMP_SOUND,
        COLLISION_SOUND,
        FAST_HEART_SOUND,

        BIRD_SOUND_COUNT
    };

    const int maxHealth = 5;
    const int invisibleTime = 2000;

    std::vector<LTexture> sTexture;
    Gun gun;
    HealthBar *health;

    Mix_Chunk *birdSound[BIRD_SOUND_COUNT];

    //-1 if not invisible
    int invisibleEndTime;
    void setInvisible(bool state);
    bool checkVisible();
    void updateVisibleState();
    void loadSound();
    void jump();

    const int maxVisibleState = 6;
    //blurry if <= maxVisibleState / 2 and clear if > maxVisibleState / 2
    int currentVisibleState;

    void loadIMG();
    void updateGPos();

    bool updateState() override;
    void renderTogRenderer() override;

public:
    MainBird(int x, int y);
    ~MainBird();

    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    void addBulletCount(GUN_TYPE gunType, int bulletCount);

    //return if health is changed
    bool changeHealth(int value);

    int getCurrentHealth() const;

    ObjectsList &getBulletList();
};

MainBird::MainBird(int x, int y) : Character(x, y, MAIN_BIRD)
{
    health = new HealthBar(mainHealthBarPosX, mainHealthBarPosY, false, true, maxHealth);
    loadIMG();
    init(x, y);
    loadSound();
}

MainBird::~MainBird()
{
    delete health;
    for (auto & sound : birdSound) {
        Mix_FreeChunk(sound);
    }
}

void MainBird::init(int x, int y)
{
    setSTexture(&sTexture);
    initCharacter(x, y);
    setLimitAngle(-45, 0);
    health->setHealth(maxHealth);
    gun.setPosAndAngle(x + getWidth() / 3, y + getHeight() / 3);
    gun.init();

    invisibleEndTime = -1;
    currentVisibleState = 0;
}

void MainBird::loadIMG()
{
    const int imgCount = 4;

    //image: https://flappybird.io/
    sTexture.resize(imgCount);
    for (int i = 0; i < imgCount - 1; i++) {
        sTexture[i].loadFromFile(mainBirdImagePath + "mainBird" + std::to_string(i) + ".png");
    }
    sTexture[imgCount - 1] = sTexture[1];
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

void MainBird::updateGPos()
{
    gMainBirdPosX = mPosX;
    gMainBirdPosY = mPosY;
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

void MainBird::loadSound()
{
    birdSound[JUMP_SOUND] = Mix_LoadWAV((mainBirdSoundPath + "jump.wav").c_str());
    birdSound[COLLISION_SOUND] = Mix_LoadWAV((mainBirdSoundPath + "collision.wav").c_str());
    birdSound[FAST_HEART_SOUND] = Mix_LoadWAV((mainBirdSoundPath + "fastheart.wav").c_str());
}

void MainBird::jump()
{
    Mix_PlayChannel((int)MAINBIRD_SOUND_CHANNEL::JUMP, birdSound[JUMP_SOUND], 0);
    setVelY(-15);
    setVelAngle(8);
}

bool MainBird::updateState()
{
    bool updated = Character::updateState();
    updateGPos();

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

bool MainBird::changeHealth(int value)
{
    //if get damaged && visible
    if (value < 0 && checkVisible()) {
        return false;
    }
    health->changeHealth(value);

    //if get damaged
    if (value < 0) {
        Mix_PlayChannel((int)MAINBIRD_SOUND_CHANNEL::COLLISION, birdSound[COLLISION_SOUND], 0);
        //check die
        if (health->getCurrentHealth() == 0) {
            onDied();
            setInvisible(false);
            return true;
        }
        else if (health->getCurrentHealth() == 1) {
            Mix_PlayChannel((int)MAINBIRD_SOUND_CHANNEL::FAST_HEART, birdSound[FAST_HEART_SOUND], 0);
        }

        setInvisible(true);
    }
    return true;
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
