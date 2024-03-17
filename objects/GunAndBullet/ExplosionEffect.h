#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Other/Object.h"

class ExplosionEffect : public Object
{
private:
    LTexture sTexture[4];

    const int imgCount = 4;
    int curIMGRender;

public:
    ExplosionEffect();
    ~ExplosionEffect();

    void init();
    void loadIMG();
    void updatePos(int x, int y, int gunWidth);
    bool render() override;
};

ExplosionEffect::ExplosionEffect() : Object(false)
{
    init();
    loadIMG();
}

ExplosionEffect::~ExplosionEffect()
= default;

void ExplosionEffect::init()
{
    //not shotting
    curIMGRender = 4;
}

void ExplosionEffect::loadIMG()
{
    sTexture[0].loadFromFile(gunImagePath + "explosionEffect0.png");
    sTexture[1].loadFromFile(gunImagePath + "explosionEffect1.png");
    sTexture[2].loadFromFile(gunImagePath + "explosionEffect2.png");
    sTexture[3].loadFromFile(gunImagePath + "explosionEffect3.png");
}

void ExplosionEffect::updatePos(int x, int y, int gunWidth)
{
    //calculate pos
    mPosX = x;
    mPosY = y;
    calculateVelocityToMouse(x, y, gunWidth);

    mPosX += x;
    mPosY += y;
}

bool ExplosionEffect::render()
{
    //is not shotting
    if (curIMGRender >= 4) {
        return false;
    }

    mTexture = &sTexture[curIMGRender];
    curIMGRender++;
    return Object::render();
}


#endif //EXPLOSIONEFFECT_H
