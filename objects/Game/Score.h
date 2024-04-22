#ifndef POINT_H
#define POINT_H

#include "../Other/Object.h"

const int scorePosX = 10;
const int scorePosY = 5;

class Score : public Object
{
    int scorePosWhenGameOverX;
    const int scorePosWhenGameOverY = 390;

    const int speed = 40;

    void setText();
    int scoreCount;
    bool moveStarted;

    bool updateState() override;

public:
    Score();
    ~Score();

    void init();
    void addBatShotCount(int value);
    void addScore(int value);
    //if false, score will return to it pos
    void startMoving(bool state);
    int getCurrentScore() const;
    bool render() override;
};

Score::Score()
{
}

void Score::init()
{
    teleportToPosX(scorePosX);
    teleportToPosY(scorePosY);

    moveStarted = false;
    scoreCount = 0;
    setText();
}

void Score::setText()
{
    //Set text color as black
    static SDL_Color textColor = { 245, 117, 66, 255 };
    mTexture->loadFromRenderedText("Score: " + std::to_string(scoreCount), glueGunFonts[FontStyle::Regular], textColor);
    scorePosWhenGameOverX = SCREEN_WIDTH / 2 - getWidth() / 2;
}

void Score::addScore(int value)
{
    scoreCount += value;
    setText();
}

int Score::getCurrentScore() const
{
    return scoreCount;
}

void Score::startMoving(bool state)
{
    moveStarted = state;
}

bool Score::updateState()
{
    if (moveStarted) {
        if (distance(mPosX, mPosY, scorePosWhenGameOverX, scorePosWhenGameOverY) <= speed) {
            mPosX = scorePosWhenGameOverX;
            mPosY = scorePosWhenGameOverY;

            return false;
        }

        int velX = mPosX,
            velY = mPosY;
        calculateVelocityBetweenTwoPos(velX, velY, scorePosWhenGameOverX, scorePosWhenGameOverY, speed);
        mPosX += velX;
        mPosY += velY;

        return true;
    }
    else {
        mPosX = scorePosX;
        mPosY = scorePosY;
    }

    return false;
}

bool Score::render()
{
    updateState();
    renderTogRenderer();
    return true;
}

Score::~Score()
= default;


#endif //POINT_H
