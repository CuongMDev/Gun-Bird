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
    //dimensions
    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual bool updateState();
    virtual void renderTogRenderer();

public:
    Object(bool init = true);
    ~Object();

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
        mTexture->free();
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

bool Object::checkCollisionObject(const Object &object)
{
    return checkCollision(mPosX, mPosY, getWidth(), getHeight(),
                          object.mPosX, object.mPosY, object.getWidth(), object.getHeight());
}

#endif //OBJECT_H
