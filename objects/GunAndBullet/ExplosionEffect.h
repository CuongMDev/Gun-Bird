#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Other/Object.h"

class ExplosionEffect : public Object
{
private:
    double mAngle;
    bool isStarting;

    void loadIMG();

public:
    ExplosionEffect();
    ~ExplosionEffect();

    void init();
    void start(int x, int y, double angle);
    bool render() override;
};

ExplosionEffect::ExplosionEffect()
{
    init();
    loadIMG();
}

ExplosionEffect::~ExplosionEffect()
= default;

void ExplosionEffect::start(int x, int y, double angle)
{
    teleportToPosX(x);
    teleportToPosY(y);
    mAngle = angle;

    isStarting = true;
}

void ExplosionEffect::init()
{
    isStarting = false;
}

void ExplosionEffect::loadIMG()
{
    mTexture->loadFromFile(gunImagePath + "explosionEffect.png");
}

bool ExplosionEffect::render()
{
    if (isStarting) {
        mTexture->render(mPosX, mPosY, NULL, mAngle);
        isStarting = false;
    }

    return true;
}


#endif //EXPLOSIONEFFECT_H
