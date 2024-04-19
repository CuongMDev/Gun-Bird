#ifndef OBJECT_H
#define OBJECT_H

#include "LTexture.h"

class Object
{
private:
    bool initialized;
protected:
    LTexture *mTexture;

    //The X and Y offsets
    int mPosX, mPosY;
    int prePosX, prePosY;

    //dimensions
    virtual int getWidth() const;
    virtual int getHeight() const;

    void teleportToPosX(int value);
    void teleportToPosY(int value);

    virtual bool updateState();
    virtual void renderTogRenderer();

public:
    Object(bool init = true);
    virtual ~Object();

    virtual bool render();
    bool checkCollisionObject(const Object &object);

    friend class ObjectsList;
};

Object::Object(bool init)
{
    initialized = init;
    if (init) {
        mTexture = new LTexture();
    }
}

Object::~Object()
{
    if (initialized) {
        delete mTexture;
    }
}

int Object::getWidth() const
{
    return mTexture->getWidth();
}

int Object::getHeight() const
{
    return mTexture->getHeight();
}

bool Object::updateState()
{
    return true;
}

void Object::renderTogRenderer()
{
    mTexture->render(mPosX, mPosY);
}

bool Object::render()
{
    //save pos
    prePosX = mPosX;
    prePosY = mPosY;

    bool stateUpdated;
    if (gamePaused) {
        stateUpdated = true;
    }
    else {
        stateUpdated = updateState();
    }

    renderTogRenderer();
    return stateUpdated;
}

void Object::teleportToPosX(int value)
{
    prePosX = mPosX = value;
}

void Object::teleportToPosY(int value)
{
    prePosY = mPosY = value;
}

bool Object::checkCollisionObject(const Object &object)
{
    int disX = mPosX - prePosX,
        disY = mPosY - prePosY;
    int objectDisX = object.mPosX - object.prePosX,
        objectDisY = object.mPosY - object.prePosY;
    //check in current pos
    bool collided = checkCollision(mPosX, mPosY, getWidth(), getHeight(),
                          object.mPosX, object.mPosY, object.getWidth(), object.getHeight());
    if (collided) return true;

    if (disX || disY || objectDisX || objectDisY) {
        //check between currentPos and prePos
        int dX1[3] = {0,disX / 2, disX},
            dY1[3] = {0,disY / 2, disY},
            dX2[3] = {0,objectDisX / 2, objectDisX},
            dY2[3] = {0,objectDisY / 2, objectDisY};

            for (int i1 = 0; i1 < 3; i1++) {
                for (int i2 = !i1; i2 < 3 - (i1 == 2); i2++) {
                    collided = checkCollision(mPosX - dX1[i1], mPosY - dY1[i1], getWidth(), getHeight(),
                                              object.mPosX - dX2[i2], object.mPosY - dY2[i2], object.getWidth(),
                                              object.getHeight());
                    if (collided) return true;
                }
            }
    }

    return false;
}

#endif //OBJECT_H
