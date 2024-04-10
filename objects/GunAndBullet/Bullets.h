#ifndef BULLETS_H
#define BULLETS_H

#include "../Other/LTexture.h"
#include "../Other/utils.h"
#include "../Scene/Pipe.h"
#include "../Other/ObjectsList.h"

enum BULLET_TYPE
{
    PISTOL_BULLET,
    GOLD_PISTOL_BULLET,
    AK47_BULLET,
    WIN94_BULLET,
    SNIPER_BULLET,

    SYRINGE_BULLET,

    BULLET_COUNT
};

struct BulletProperties
{
    double speed;

    std::string imageName;
};

class Bullets : public Object
{
private:
    static const BulletProperties bulletProperties[BULLET_COUNT];
    static LTexture sTexture[BULLET_COUNT];

    BULLET_TYPE currentBullet;

    //use to save desX, desY
    int sDesX, sDesY;

    int mDamage;
    //The velocity
    int mVelX, mVelY;
    //Angle
    double mAngle;

    bool checkOutTheBorder();
    bool move();

    void loadIMG();
    bool noNeedToCalculateVel;
    void calculateVelocity();

    bool updateState() override;
    void renderTogRenderer() override;

protected:
    void init(int x, int y, BULLET_TYPE bulletType, int damage, int desX, int desY);

public:
    Bullets(int x, int y, BULLET_TYPE bulletType, int damage, int desX = -1, int desY = -1);
    ~Bullets();

    int getDamage();
};

const BulletProperties Bullets::bulletProperties[] = {
        {17, "pistol.png"}, //pistol
        {33, "goldpistol.png"}, //gold pistol
        {20, "ak47.png"}, //AK47
        {30, "win94.png"}, //Win94
        {50, "sniper.png"}, //Sniper

        {20, "syringe.png"}, //Syringe
};

LTexture Bullets::sTexture[];

//desPos = -1 if desPos = mousePos
Bullets::Bullets(int x, int y, BULLET_TYPE bulletType, int damage, int desX, int desY) : Object(false)
{
    loadIMG();
    init(x, y, bulletType, damage, desX, desY);
}

Bullets::~Bullets()
=default;

void Bullets::init(int x, int y, BULLET_TYPE bulletType, int damage, int desX = -1, int desY = -1)
{
    mDamage = damage;

    noNeedToCalculateVel = false;

    currentBullet = bulletType;
    mTexture = &sTexture[currentBullet];

    sDesX = desX; sDesY = desY;
    if (sDesX == -1) {
        cursorMouse->getAimPos(sDesX, sDesY);
    }

    mPosX = x;
    mPosY = y;

    mAngle = angleBetweenTwoPos(mPosX, mPosY, sDesX, sDesY);

    calculateVelocity();
}

bool Bullets::checkOutTheBorder()
{
    if (mPosX > SCREEN_WIDTH || mPosX + getWidth() < 0) {
        return true;
    }
    if (mPosY > SCREEN_HEIGHT || mPosY + getHeight() < 0) {
        return true;
    }

    return false;
}

void Bullets::loadIMG()
{
    static bool loadedIMG = false;
    if (loadedIMG) {
        return;
    }
    loadedIMG = true;

    //image: https://midnitepixelated.itch.io/pixel-Bullets
    for (int bulletType = 0; bulletType < BULLET_COUNT; bulletType++) {
        sTexture[bulletType].loadFromFile(bulletImagePath + bulletProperties[bulletType].imageName, true, 0, 0, 0);
    }
}

void Bullets::calculateVelocity()
{
    if (noNeedToCalculateVel) {
        return;
    }
    //calculate Velocity
    mVelX = mPosX + getWidth() / 2, mVelY = mPosY + getHeight();
    calculateVelocityBetweenTwoPos(mVelX, mVelY, sDesX, sDesY, bulletProperties[currentBullet].speed);

    if (distance(mPosX, mPosY, sDesX, sDesY) < std::max(100, getWidth() + getHeight())) {
        noNeedToCalculateVel = true;
    }
}

bool Bullets::move()
{
    mPosX += mVelX;
    mPosY += mVelY;

    if (checkOutTheBorder()) return false;
    return true;
}

int Bullets::getDamage()
{
    return mDamage;
}

bool Bullets::updateState() {
    calculateVelocity();
    if (move()) return true;
    return false;
}

void Bullets::renderTogRenderer()
{
    mTexture->render(mPosX, mPosY, NULL, mAngle);
}

#endif