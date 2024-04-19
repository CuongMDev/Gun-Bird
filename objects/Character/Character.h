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

    SDL_RendererFlip flipMode;

    //border dimension
    int borderPosX;
    int borderPosY;
    int borderWidth;
    int borderHeight;

    double limitAngleUpper;
    double limitAngleLower;

    //check whether character died
    bool died;

    //check if exist gravity
    bool gravity;

    void initType();
    void setToBorderPos();
    void increaseTimeRender();
    void resetIMGRender();

    bool checkOutBorder();
    bool checkGroundCollision();

    bool move();
    bool playingUpdate();
    bool dyingUpdate();

protected:
    Character(int x, int y, CHARACTER_TYPE character);
    ~Character();

    void initCharacter(int x, int y);

    //use when have gravitation
    void decreaseVelAndAngle();
    bool isStayingOnGround();

    bool updateState() override;
    void renderTogRenderer() override;
    void setSTexture(std::vector<LTexture> *newSTexture);
    int getCurrentIMG() const;
    bool checkEndOfIMG();

    void setLimitAngle(int limitAngleLower, int limitAngleUpper);
    void setGravity(bool state);
    void setFlipMode(SDL_RendererFlip _flipMode);
    void setBorder(int x, int y, int w, int h);
    void setAlpha(Uint8 alpha);

    virtual void setAngle(double value);
    void setVelX(int value);
    void setVelY(int value);
    void setVelAngle(double value);
    void addVelX(int value);
    void addVelY(int value);
    void addVelAngle(double value);

public:
    virtual void onDied();
    bool isDied() const;

    friend class Menu;
};

Character::Character(int x, int y, CHARACTER_TYPE character) : Object(false)
{
    mCharacterType = character;

    initType();
}

Character::~Character()
= default;

void Character::initCharacter(int x, int y)
{
    resetIMGRender();

    died = false;

    imgChangeVel = 5;

    //Initialize the offsets
    teleportToPosX(x);
    teleportToPosY(y);

    mAngle = 0;
    flipMode = SDL_FLIP_NONE;
    //Create the necessary SDL_Rects
    //mColliders.resize(11);

    limitAngleUpper = 180;
    limitAngleLower = -180;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    mVelAngle = 0;

    //init border dimension
    setBorder(0, 0, SCREEN_WIDTH, groundPosY + 10);

    gravity = true;

    switch (mCharacterType) {
        case MAIN_BIRD:
            limitAngleUpper = 0;
            limitAngleLower = -45;
            break;
        case BAT:

            break;
        case BOSS:
            limitAngleUpper = 180;
            limitAngleLower = -180;
            break;

        default:
            break;
    }
}

void Character::onDied()
{
    if (died) {
        return;
    }

    died = true;

    //continue moving
    mVelX += gCurVelocityYScene - gVelocityYScene;
    //turn on gravity
    setGravity(true);

    setBorder(borderPosX, 0, SCREEN_WIDTH + borderPosX, groundPosY + getHeight() / 2);
    if (direction == RIGHT) {
        limitAngleUpper = 90;
    }
    else {
        limitAngleLower = -90;
    }
}

void Character::initType()
{
    sTexture = new std::vector<LTexture>;
    switch (mCharacterType) {
        case MAIN_BIRD:
            direction = RIGHT;
            initImgChangeVelWhenAir = 2;

            break;
        case BAT:
            direction = LEFT;
            initImgChangeVelWhenAir = 6;

            break;

        case BOSS:
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
    mTexture->render(mPosX, mPosY, NULL, mAngle, NULL, flipMode);
}

void Character::setSTexture(std::vector<LTexture> *newSTexture)
{
    sTexture = newSTexture;
    imgCount = (int)sTexture->size();

    resetIMGRender();
}

void Character::resetIMGRender()
{
    mTexture = &(*sTexture)[0];
    curTimeRender = 0;
    curIMGRender = 0;
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
    if (!gravity) return;

    if (!isStayingOnGround()) {
        addVelY(1);
        //Increase Y velocity because of gravitation
    }
    if (direction == RIGHT) {
        addVelAngle(-0.5);
    }
    else { //LEFT
        addVelAngle(0.5);
    }
}

bool Character::move()
{
    bool moved = false;
    if (mVelX > gVelocityYScene) moved = true;

    mPosX += mVelX;
    mPosY += mVelY;
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
            mPosY = borderPosY + borderHeight - getHeight();
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

void Character::setLimitAngle(int limitAngleLower, int limitAngleUpper)
{
    this->limitAngleLower =limitAngleLower;
    this->limitAngleUpper =limitAngleUpper;
}

void Character::setGravity(bool state)
{
    gravity = state;
}

void Character::setFlipMode(SDL_RendererFlip _flipMode)
{
    flipMode = _flipMode;
}

void Character::setBorder(int x, int y, int w, int h)
{
    borderPosX = x;
    borderPosY = y;
    borderWidth = w;
    borderHeight = h;
}

void Character::setAlpha(Uint8 alpha)
{
    for (int img = 0; img < imgCount; img++) {
        (*sTexture)[img].setAlpha(alpha);
    }
}

void Character::setAngle(double value)
{
    mAngle = value;
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

int Character::getCurrentIMG() const
{
    int answer = curIMGRender - 1;
    if (answer == -1) answer += imgCount;
    return answer;
}

bool Character::checkEndOfIMG()
{
    return (curTimeRender == imgChangeVel - 1);
}

#endif