#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "../Other/Object.h"

class HealthBar : public Object
{
private:
    enum HEALTH_BAR_TYPE
    {
        HEALTH_BAR_SHELL,
        HEALTH_BAR_STATE_PLAYER,
        HEALTH_BAR_STATE_ENEMY,

        HEALTH_BAR_COUNT
    };
    static LTexture sTexture[HEALTH_BAR_COUNT];

    const int HPWidth = 30;
    const int HPHeight = 7;

    bool mHaveShell;
    bool mInEnemySide;

    int mMaxHP;
    int mCurrentHP;

    void loadIMG();
    void renderTogRenderer() override;

public:
    HealthBar(int x, int y, bool inEnemySide, bool haveShell, int maxHP);
    ~HealthBar();

    void init(int x, int y, bool inEnemySide, bool haveShell, int maxHP);

    void updatePos(int x, int y);
    void setHealth(int value);
    void setMaxHealth(int value);
    void changeHealth(int value);
    void changeMaxHealth(int value);

    int getCurrentHealth() const;
};

LTexture HealthBar::sTexture[] = {};

HealthBar::HealthBar(int x, int y, bool inEnemySide, bool haveShell, int maxHP) : Object(false)
{
    loadIMG();
    init(x, y, inEnemySide, haveShell, maxHP);
}

HealthBar::~HealthBar()
= default;

void HealthBar::init(int x, int y, bool inEnemySide, bool haveShell, int maxHP)
{
    mPosX = x;
    mPosY = y;
    mInEnemySide = inEnemySide;
    mHaveShell = haveShell;
    mMaxHP = mCurrentHP = maxHP;

    mTexture = &sTexture[HEALTH_BAR_STATE_PLAYER];
}

void HealthBar::updatePos(int x, int y)
{
    mPosX = x - getWidth() / 2;
    mPosY = y;
}

void HealthBar::setHealth(int value)
{
    mCurrentHP = value;
    if (mCurrentHP < 0) mCurrentHP = 0;
    if (mCurrentHP > mMaxHP) mCurrentHP = mMaxHP;
}

void HealthBar::setMaxHealth(int value) {
    mMaxHP = value;
    if (mMaxHP < 0) mMaxHP = 0;
}

void HealthBar::changeMaxHealth(int value)
{
    mMaxHP += value;
    if (mMaxHP < 0) mMaxHP = 0;
}

void HealthBar::changeHealth(int value)
{
    mCurrentHP += value;
    if (mCurrentHP < 0) mCurrentHP = 0;
    if (mCurrentHP > mMaxHP) mCurrentHP = mMaxHP;
}

void HealthBar::loadIMG()
{
    static bool loadedIMG = false;
    if (loadedIMG) {
        return;
    }
    loadedIMG = true;

    sTexture[HEALTH_BAR_SHELL].loadFromFile(healthBarImagePath + "healthbarshell.png");
    sTexture[HEALTH_BAR_STATE_PLAYER].loadFromFile(healthBarImagePath + "healthbarstatePlayer.png");
    sTexture[HEALTH_BAR_STATE_ENEMY].loadFromFile(healthBarImagePath + "healthbarstateEnemy.png");
}

void HealthBar::renderTogRenderer()
{
    int barLength = (getWidth() * mCurrentHP + mMaxHP - 1) / mMaxHP;
    SDL_Rect clip;
    clip.x = 0, clip.y = 0;
    clip.w = barLength, clip.h = getHeight();
    if (!mInEnemySide) {
        if (mHaveShell) {
            sTexture[HEALTH_BAR_SHELL].render(mPosX, mPosY);
        }
        sTexture[HEALTH_BAR_STATE_PLAYER].render(mPosX + HPWidth, mPosY + HPHeight, &clip);
    }
    else {
        if (mHaveShell) {
            sTexture[HEALTH_BAR_SHELL].render(mPosX, mPosY, NULL);
            sTexture[HEALTH_BAR_STATE_ENEMY].render(mPosX+ HPWidth, mPosY + HPHeight, &clip);
        }
        else {
            sTexture[HEALTH_BAR_STATE_ENEMY].render(mPosX, mPosY, &clip, 0, NULL, SDL_FLIP_NONE, -70);
        }
    }
}

int HealthBar::getCurrentHealth() const
{
    return mCurrentHP;
}

#endif //HEALTHBAR_H
