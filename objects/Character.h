#ifndef CHARACTER_H
#define CHARACTER_H

#include "Ground.h"

const int mainBirdPosX = 200;
const int mainBirdPosY = SCREEN_HEIGHT / 2;

enum CHARACTER_TYPE
{
    MAIN_BIRD,
    BAT,

    CHARACTER_COUNT
};

enum DIRECTION
{
    LEFT,
    RIGHT
};

class Character
{
private:
    LTexture mTexture[4];

    CHARACTER_TYPE mCharacterType;
    DIRECTION direction;

    int initImgChangeVelWhenFlying;

    int curTimeRender;
    int curIMGRender;
    int imgChangeVel;
    //The X and Y offsets
    int mPosX, mPosY;
    //The angle
    double mAngle;
    //The velocity
    int mVelX, mVelY;
    double mVelAngle;

    double limitAngleUpper;
    double limitAngleLower;

    //check whether character died
    bool died;

    void loadIMG();
    void setPosToBorderPos();

    bool checkOutBorder();
    bool checkGroundCollision();

    bool isStayingOnGround();
    bool move();
    bool playingRender();
    bool dyingRender();

public:
    Character(int x, int y, CHARACTER_TYPE character);
    ~Character();

    void init(int x, int y);

    //use when have gravitation
    void decreaseVelAndAngle();

    void onDied();
    bool isDied() const;
    bool render();

    //dimensions
    int getPosX() const;
    int getPosY() const;
    int getWidth();
    int getHeight();

    void setVelX(int value);
    void setVelY(int value);
    void setVelAngle(double value);
    void addVelX(int value);
    void addVelY(int value);
    void addVelAngle(double value);
};

Character::Character(int x, int y, CHARACTER_TYPE character)
{
    mCharacterType = character;

    init(x, y);
    loadIMG();
}

Character::~Character()
{
    for (int i = 0; i < 2; i++) {
        mTexture[i].free();
    }
}

void Character::init(int x, int y)
{
    died = false;

    curTimeRender = 0;
    curIMGRender = 0;
    imgChangeVel = 5;

    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    mAngle = 0;
    //Create the necessary SDL_Rects
    //mColliders.resize(11);

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    mVelAngle = 0;

    if (direction == RIGHT) {
        limitAngleUpper = 0;
        limitAngleLower = -45;
    }
    else if (direction == LEFT) {
        limitAngleUpper = 45;
        limitAngleLower = 0;
    }
}

void Character::onDied()
{
    if (died) {
        return;
    }

    died = true;

    //continue moving
    mVelX = gInitVelocityYScene - gVelocityYScene;
    mVelY = 15;

    if (direction == RIGHT) {
        limitAngleUpper = 90;
    }
    else {
        limitAngleLower = -90;
    }
}

void Character::loadIMG()
{
    //image: https://flappybird.io/,
    switch (mCharacterType) {
        case MAIN_BIRD:
            mTexture[0].loadFromFile(imagePath + "mainBird0.png", true, 124, 197, 205);
            mTexture[1].loadFromFile(imagePath + "mainBird1.png", true, 124, 197, 205);
            mTexture[2].loadFromFile(imagePath + "mainBird2.png", true, 124, 197, 205);
            mTexture[3] = mTexture[1];

            direction = RIGHT;
            initImgChangeVelWhenFlying = 2;

            break;
        case BAT:
            mTexture[0].loadFromFile(imagePath + "bat0.png", true, 34,177,76);
            mTexture[1].loadFromFile(imagePath + "bat1.png", true, 34,177,76);
            mTexture[2].loadFromFile(imagePath + "bat2.png", true, 34,177,76);
            mTexture[3] = mTexture[1];

            direction = LEFT;
            initImgChangeVelWhenFlying = 6;

            break;
        default:
            break;
    }
}

void Character::setPosToBorderPos()
{
    if (mPosX < 0) {
        mPosX = 0;
    }
    if (mPosX + mTexture[curIMGRender].getWidth() > SCREEN_WIDTH) {
        mPosX = SCREEN_WIDTH - mTexture[curIMGRender].getWidth();
    }
    if (mPosY + mTexture[curIMGRender].getHeight() < 0) {
        mPosY = -mTexture[curIMGRender].getHeight();
    }
}

bool Character::playingRender()
{
    bool moved = false;
    if (move()) {
        moved = true;
    }
    //change wing flapping speed
    if (isStayingOnGround()) {
        imgChangeVel = 5;
    }
    else {
        imgChangeVel = initImgChangeVelWhenFlying;
    }

    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);

    //increase time render
    curTimeRender++;
    if (curTimeRender >= imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender == 4) {
            curIMGRender = 0;
        }
    }

    return moved;
}

bool Character::dyingRender()
{
    //start falling
    decreaseVelAndAngle();

    if (mVelX > gVelocityYScene && isStayingOnGround()) {
        //Velocity X decreases due to friction
        mVelX--;
    }

    bool moved = move();
    mTexture[curIMGRender].render(mPosX, mPosY, NULL, mAngle);
    return moved;
}

bool Character::isStayingOnGround()
{
    return (mPosY + mTexture[curIMGRender].getHeight() == groundPosY + 10);
}

bool Character::checkGroundCollision()
{
    if (mPosY + mTexture[curIMGRender].getHeight() >= groundPosY + 10) {
        return true;
    }
    return false;
}

bool Character::checkOutBorder()
{
    if (mPosX < 0) {
        return true;
    }
    if (mPosX + mTexture[curIMGRender].getWidth() > SCREEN_WIDTH) {
        return true;
    }
    if (mPosY + mTexture[curIMGRender].getHeight() < 0) {
        return true;
    }
    return false;
}

void Character::decreaseVelAndAngle()
{
    if (!isStayingOnGround()) {
        addVelY(-1);
        //Decrease Y velocity because of gravitation
    }
    addVelAngle(-0.5);
}

bool Character::move()
{
    bool moved = false;
    if (mVelX > gVelocityYScene) moved = true;

    mPosX += mVelX;
    mPosY -= mVelY;
    mAngle -= mVelAngle;

    if (mAngle > limitAngleUpper) {
        mAngle = limitAngleUpper;
    }
    if (mAngle < limitAngleLower) {
        mAngle = limitAngleLower;
    }

    //do not lower head
    if (mAngle == 0) {
        mVelAngle = 0;
    }

    if (checkGroundCollision()) {
        //not staying on the ground
        if (!isStayingOnGround()) {
            //set pos to ground pos
            mPosY = groundPosY + 10 - mTexture[curIMGRender].getHeight();
            moved = true;
        }
        else {
            //continue to fall
            mPosY += mVelY;
        }
        //reset Y velocity
        mVelY = 0;
    }
    else {
        moved = true;
    }

    if (checkOutBorder()) {
        // set pos to border pos
        setPosToBorderPos();
        if (isStayingOnGround()) {
            //reset Y velocity
            mVelY = 0;
        }
    }

    return moved;
}

bool Character::isDied() const
{
    return died;
}

bool Character::render()
{
    bool rendered = true;
    if (!died) {
        playingRender();
    }
    else {
        rendered = dyingRender();
    }

    return rendered;
}

int Character::getWidth()
{
    return mTexture[curIMGRender].getWidth();
}

int Character::getHeight()
{
    return mTexture[curIMGRender].getHeight();
}

int Character::getPosX() const
{
    return mPosX;
}

int Character::getPosY() const
{
    return mPosY;
}

void Character::setVelX(int value)
{
    mVelX = value;
}

void Character::setVelY(int value)
{
    mVelY = value;
}

void Character::addVelX(int value)
{
    mVelX += value;
}

void Character::addVelY(int value)
{
    mVelY += value;
}

void Character::setVelAngle(double value)
{
    mVelAngle = value;
}

void Character::addVelAngle(double value)
{
    mVelAngle += value;
}

#endif