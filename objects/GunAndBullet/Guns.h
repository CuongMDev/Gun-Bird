#ifndef GUNS_H
#define GUNS_H

#include "Bullets.h"
#include "../CursorMouse/CursorMouse.h"
#include "ExplosionEffect.h"
#include <cmath>
#include <list>

const int gunPosX = 5;
const int gunPosY = groundPosY + 10;

enum GUN_TYPE
{
    PISTOL,
    SILENT_PISTOL,
    GOLD_PISTOL,
    AK47,
    SNIPER,

    GUN_COUNT
};

struct GunProperties
{
    int damage;
    int timeRestoreAim;
    int recoil;
    int maxVelRecoil;
    int shootDelay;

    BULLET_TYPE bulletType;

    std::string imageName;
};

class Guns : private Object
{
private:
    static const GunProperties gunProperties[GUN_COUNT];
    static LTexture sTexture[GUN_COUNT];

    ObjectsList bulletsList;
    GUN_TYPE currentGun;

    //The angle
    double mAngle;

    int curBullet;
    int bulletInTape;

    //shot when curTime=shotDelay
    int curTime;

    int curTimeRestoreAim;

    int mVelRecoil;

    bool mouseHold;

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
    void restoreAim();
    //check and shoot
    void shoot();

public:
    Guns();

    ObjectsList &getBulletList();

    void init();
    bool render() override;
    void handleEvent(SDL_Event *e);
    void setPosAndAngle(int x, int y);
    void changeGun(GUN_TYPE gunType);
};

const GunProperties Guns::gunProperties[] = {
        {3, 17, 1, 3, 15, PISTOL_BULLET, "pistol.png"}, //pistol
        {3, 17, 1, 3, 10, PISTOL_BULLET, "silentpistol.png"}, //silent pistol
        {7, 17,1,  5, 15, GOLD_PISTOL_BULLET, "goldpistol.png"}, //gold pistol
        {5, 7, 1, 7, 5, AK47_BULLET, "ak47.png"}, //AK47
        {12, 30, 20, 20, 30, SNIPER_BULLET, "sniper.png"}, //Sniper
};

LTexture Guns::sTexture[];

Guns::Guns() : Object(false)
{
    loadGunIMG();
    init();
}

ObjectsList &Guns::getBulletList()
{
    return bulletsList;
}

void Guns::init()
{
    mPosX = 0;
    mPosY = 0;
    mAngle = 0;
    mVelRecoil = 0;
    mouseHold = false;

    currentGun = PISTOL;
    mTexture = &sTexture[currentGun];

    bulletsList.reset();
}

void Guns::loadGunIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    for (auto gunType = 0; gunType < GUN_COUNT; gunType++) {
        sTexture[gunType].loadFromFile(gunImagePath + gunProperties[gunType].imageName, true, 67, 76, 111);
    }
}

void Guns::turnTowards(int mouseX, int mouseY)
{
    double dx = mouseX - mPosX;
    double dy = mouseY - mPosY;
    double angle = atan2(dy, dx) * 180 / M_PI;

    mAngle = angle;
}

void Guns::updateAngle()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    turnTowards(mouseX, mouseY);
}

void Guns::restoreAim()
{
    if (curTimeRestoreAim > 0) {
        curTimeRestoreAim--;
        if (curTimeRestoreAim % gunProperties[currentGun].timeRestoreAim == 0) {
            CursorMouse::move(0, 10);
        }
    }

    if (curTimeRestoreAim % gunProperties[currentGun].timeRestoreAim == 0) {
        if (mVelRecoil > 0) {
            //restore gun aim
            mVelRecoil--;
        }
    }
}


void Guns::shoot()
{
    if (curTime < gunProperties[currentGun].shootDelay) {
        curTime++;
    }

    if (!mouseHold) {
        restoreAim();
        return;
    }

    if (curTime < gunProperties[currentGun].shootDelay) {
        //It's not time to shoot yet
        return;
    }

    //add bullet
    bulletsList.add(new Bullets(mPosX, mPosY + pivotY, getWidth(), mAngle, gunProperties[currentGun].bulletType));

    //increase time restore aim
    if (curTimeRestoreAim < gunProperties[currentGun].timeRestoreAim * 2) {
        curTimeRestoreAim += gunProperties[currentGun].timeRestoreAim;
    }

    mVelRecoil = std::min(mVelRecoil + gunProperties[currentGun].recoil, gunProperties[currentGun].maxVelRecoil);
    //reset
    curTime = 0;
    //recoil
    if (mVelRecoil < 3) {
        //3 first bullets have little recoil
        CursorMouse::recoilMouse(1, 10);
    }
    else {
        CursorMouse::recoilMouse(1, 10 * mVelRecoil);
    }
}

void Guns::handleEvent(SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_1:
                changeGun(PISTOL);
                break;
            case SDLK_2:
                changeGun(SILENT_PISTOL);
                break;
            case SDLK_3:
                changeGun(GOLD_PISTOL);
                break;
            case SDLK_4:
                changeGun(AK47);
                break;
            case SDLK_5:
                changeGun(SNIPER);
                break;

            default:
                break;
        }
    }

    if (e->type == SDL_MOUSEMOTION) {
        updateAngle();
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            //left mouse
            mouseHold = true;
        }
    }
    else if (e->type == SDL_MOUSEBUTTONUP) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            //left mouse
            mouseHold = false;
        }
    }
}

bool Guns::render()
{
    shoot();
    bulletsList.renderAll();

    SDL_Point center{ pivotX, pivotY };
    mTexture->render(mPosX, mPosY, NULL, mAngle, &center);

    return true;
}

void Guns::setPosAndAngle(int x, int y)
{
    mPosX = x;
    mPosY = y;
    updateAngle();
}

void Guns::changeGun(GUN_TYPE gunType)
{
    currentGun = gunType;
    mTexture = &sTexture[currentGun];
}

#endif