#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Scene/Ground.h"
#include "../Other/ObjectsList.h"

enum CHARACTER_TYPE
{
    MAIN_BIRD,
    BAT,
    BOSS,

    CHARACTER_COUNT
};

enum DIRECTION
{
    LEFT,
    RIGHT
};

class Character : public Object
{
private:
    std::vector<LTexture> *sTexture;

    CHARACTER_TYPE mCharacterType;
    DIRECTION direction;

    int initImgChangeVelWhenAir;
    int imgCount;
    int curTimeRender;
    int curIMGRender;
    int imgChangeVel;
    //The angle
    double mAngle;
    //The velocity
    int mVelX, mVelY;
    double mVelAngle;

    //border dimension
    int borderPosX;
    int borderPosY;
    int borderWidth;
    int borderHeight;

    double limitAngleUpper;
    double limitAngleLower;

    //check whether character died
    bool died;

    void loadIMG();
    void setToBorderPos();
    void increaseTimeRender();

    bool checkOutBorder();
    bool checkGroundCollision();

    bool isStayingOnGround();
    bool move();
    bool playingUpdate();
    bool dyingUpdate();

protected:
    Character(int x, int y, CHARACTER_TYPE character);
    ~Character();


    void initCharacter(int x, int y);

    //use when have gravitation
    void decreaseVelAndAngle();

    void onDied();
    bool updateState() override;
    void renderTogRenderer() override;

    void setAlpha(Uint8 alpha);
    void setVelX(int value);
    void setVelY(int value);
    void setVelAngle(double value);
    void addVelX(int value);
    void addVelY(int value);
    void addVelAngle(double value);

public:
    bool isDied() const;
};

Character::Character(int x, int y, CHARACTER_TYPE character) : Object(false)
{
    mCharacterType = character;

    loadIMG();
    initCharacter(x, y);
}

Character::~Character()
{
    if (mCharacterType == BOSS) {
        return;
    }
    delete sTexture;
}

void Character::initCharacter(int x, int y)
{
    mTexture = &(*sTexture)[0];

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

    //init border dimension
    borderPosX = 0;
    borderPosY = 0;
    borderWidth = SCREEN_WIDTH;
    borderHeight = groundPosY + 10;

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
    mVelX += gInitVelocityYScene - gVelocityYScene;

    if (direction == RIGHT) {
        limitAngleUpper = 90;
    }
    else {
        limitAngleLower = -90;
    }
}

void Character::loadIMG()
{
    sTexture = new std::vector<LTexture>;
    switch (mCharacterType) {
        //image: https://flappybird.io/
        case MAIN_BIRD:
            imgCount = 4;
            sTexture->resize(imgCount);

            (*sTexture)[0].loadFromFile(mainBirdImagePath + "mainBird0.png", true, 124, 197, 205);
            (*sTexture)[1].loadFromFile(mainBirdImagePath + "mainBird1.png", true, 124, 197, 205);
            (*sTexture)[2].loadFromFile(mainBirdImagePath + "mainBird2.png", true, 124, 197, 205);
            (*sTexture)[3] = (*sTexture)[1];

            direction = RIGHT;
            initImgChangeVelWhenAir = 2;

            break;
            //image: https://opengameart.org/content/bat-rework
        case BAT:
            imgCount = 4;
            sTexture->resize(imgCount);

            (*sTexture)[0].loadFromFile(batImagePath + "bat0.png", true, 34, 177, 76);
            (*sTexture)[1].loadFromFile(batImagePath + "bat1.png", true, 34, 177, 76);
            (*sTexture)[2].loadFromFile(batImagePath + "bat2.png", true, 34, 177, 76);
            (*sTexture)[3] = (*sTexture)[1];

            direction = LEFT;
            initImgChangeVelWhenAir = 6;

            break;

        case BOSS:
            imgCount = 8;
            sTexture->resize(imgCount);
            for (int i = 0; i < imgCount; i++) {
                (*sTexture)[i].loadFromFile(dragonImagePath + "Move/move" + std::to_string(i) + ".png", true, 34,177,76);
            }

            direction = LEFT;
            initImgChangeVelWhenAir = 5;
        default:
            break;
    }
}

void Character::setToBorderPos()
{
    if (mPosX < borderPosX) {
        mPosX = borderPosX;
    }
    if (mPosX + getWidth() > borderWidth) {
        mPosX = borderWidth - getWidth();
    }
    if (mPosY + getHeight() < borderPosY) {
        mPosY = borderPosY - getHeight();
    }
}

void Character::increaseTimeRender()
{
    curTimeRender++;
    if (curTimeRender >= imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender >= imgCount) {
            curIMGRender = 0;
        }
    }
}

bool Character::playingUpdate()
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
        imgChangeVel = initImgChangeVelWhenAir;
    }

    mTexture = &(*sTexture)[curIMGRender];
    increaseTimeRender();

    return moved;
}

bool Character::dyingUpdate()
{
    //start falling
    decreaseVelAndAngle();

    if (mVelX != -gVelocityYScene && isStayingOnGround()) {
        //Velocity X changes until equal Scene Velocity
        if (mVelX > -gVelocityYScene) mVelX--;
        else mVelX++;
    }

    bool moved = move();
    return moved;
}

bool Character::updateState()
{
    bool updated = true;
    if (!died) {
        playingUpdate();
    }
    else {
        updated = dyingUpdate();
    }

    return updated;
}

void Character::renderTogRenderer()
{
    mTexture->render(mPosX, mPosY, NULL, mAngle);
}

bool Character::isStayingOnGround()
{
    return (mPosY + getHeight() == borderPosY + borderHeight);
}

bool Character::checkGroundCollision()
{
    if (mPosY + getHeight() >= borderPosY + borderHeight) {
        return true;
    }
    return false;
}

bool Character::checkOutBorder()
{
    if (mPosX < 0) {
        return true;
    }
    if (mPosX + getWidth() > SCREEN_WIDTH) {
        return true;
    }
    if (mPosY + getHeight() < 0) {
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
            mPosY = groundPosY + 10 - getHeight();
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
        setToBorderPos();
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

void Character::setAlpha(Uint8 alpha)
{
    for (int img = 0; img < imgCount; img++) {
        (*sTexture)[img].setAlpha(alpha);
    }
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