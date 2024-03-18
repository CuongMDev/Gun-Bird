#ifndef GUNS_H
#define GUNS_H

#include "Bullets.h"
#include "../CursorMouse/CursorMouse.h"
#include "ExplosionEffect.h"
#include "../Other/Font.h"
#include <cmath>
#include <list>

const int gunPosX = 5;
const int gunPosY = groundPosY + 10;

const int typePosX = 25;
const int typePosY = 50;

const int bulletTextPosX = 25;
const int bulletTextPosY = 80;

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
    int bulletInTape;

    BULLET_TYPE bulletType;

    std::string imageName;
};

class Guns : private Object
{
private:
    static const GunProperties gunProperties[GUN_COUNT];
    static LTexture sTexture[GUN_COUNT];
    static const int reloadingTime;

    LTexture bulletLabel;

    int curBullet[GUN_COUNT];
    int bulletLeft[GUN_COUNT];

    ObjectsList bulletsList;
    GUN_TYPE currentGun;

    ExplosionEffect explosionEffect;

    //-1 if not reloading
    int reloadingEndTime;

    //The angle
    double mAngle;

    //shoot when curTime=shootDelay
    int curTime;

    int curTimeRestoreAim;

    int mVelRecoil;

    bool mouseHold;

    void loadGunIMG();
    void turnTowards(int mouseX, int mouseY);
    void updateAngle();
    void addBullet();
    void calculateGunHeadPos(int &x, int &y);
    void restoreAim();
    void updateBulletText();
    //check and shoot
    void shoot();
    void checkReloading();

public:
    Guns();

    ObjectsList &getBulletList();

    void init();
    bool render() override;
    void handleEvent(SDL_Event *e);
    void setPosAndAngle(int x, int y);
    void changeGun(GUN_TYPE gunType);
    void reload();
};

const GunProperties Guns::gunProperties[] = {
        {3, 17, 1, 3, 15, 7, PISTOL_BULLET, "pistol.png"}, //pistol
        {3, 17, 1, 3, 10, 7, PISTOL_BULLET, "silentpistol.png"}, //silent pistol
        {7, 17,1,  5, 15, 1,GOLD_PISTOL_BULLET, "goldpistol.png"}, //gold pistol
        {5, 7, 1, 7, 5, 30,AK47_BULLET, "ak47.png"}, //AK47
        {12, 30, 20, 20, 30, 5, SNIPER_BULLET, "sniper.png"}, //Sniper
};
const int Guns::reloadingTime = 2000;

LTexture Guns::sTexture[];

Guns::Guns() : Object(false)
{
    loadGunIMG();
    init();
    updateBulletText();
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
    reloadingEndTime = -1;
    explosionEffect.init();

    currentGun = PISTOL;
    mTexture = &sTexture[currentGun];

    memset(curBullet, 0, sizeof(curBullet));
    memset(bulletLeft, 0, sizeof(bulletLeft));

    curBullet[currentGun] = gunProperties[currentGun].bulletInTape;
    bulletLeft[currentGun] = -1; //infinity
    updateBulletText();

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

void Guns::addBullet()
{
    int x, y;
    calculateGunHeadPos(x, y);

    //Adjust the bullet to fit into the barrel of the gun
    explosionEffect.start(x, y - 10, mAngle);
    bulletsList.add(new Bullets(x, y - 5, mAngle, gunProperties[currentGun].bulletType));

    curBullet[currentGun]--;
    updateBulletText();
}

void Guns::calculateGunHeadPos(int &x, int &y)
{
    x = mPosX;
    y = mPosY + pivotY;
    calculateVelocityToMouse(x, y, getWidth());

    x += mPosX;
    y += mPosY + pivotY;
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

void Guns::updateBulletText()
{
    std::string bulletLeftText;
    //infinity
    if (bulletLeft[currentGun] == -1) {
        bulletLeftText = "oo";
    }
    else {
        bulletLeftText = std::to_string(bulletLeft[currentGun]);
    }
    // A/B
    bulletLabel.loadFromRenderedText(std::to_string(curBullet[currentGun]) + '/' + bulletLeftText, firaFonts[FontStyle::Bold], {0, 0, 0});
}

void Guns::shoot()
{
    //no bullet
    if (curBullet[currentGun] == 0) {
        if (!mouseHold) {
            reload();
        }
        return;
    }
    //is reloading
    if (reloadingEndTime != -1) {
        return;
    }

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

    addBullet();

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

void Guns::checkReloading()
{
    if (reloadingEndTime == -1) {
        return;
    }
    if (SDL_GetTicks() <= reloadingEndTime) {
        return;
    }

//------------------------------------------------
    //reset
    reloadingEndTime = -1;

    //starting reloading
    if (bulletLeft[currentGun] == -1) {
        curBullet[currentGun] = gunProperties[currentGun].bulletInTape;
    } //infinity
    else {
        int addCount = std::min(bulletLeft[currentGun], gunProperties[currentGun].bulletInTape - curBullet[currentGun]);
        curBullet[currentGun] += addCount;
        bulletLeft[currentGun] -= addCount;
    }

    updateBulletText();
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

            case SDLK_r:
                reload();
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
    checkReloading();

    //show type
    mTexture->render(typePosX, typePosY);
    //show bullet
    bulletLabel.render(bulletTextPosX, bulletTextPosY);

    shoot();
    bulletsList.renderAll();

    SDL_Point center{ pivotX, pivotY };
    mTexture->render(mPosX, mPosY, NULL, mAngle, &center);
    explosionEffect.render();

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

    updateBulletText();
}

void Guns::reload()
{
    //no bullet
    if (bulletLeft[currentGun] == 0) {
        return;
    }
    if (reloadingEndTime == -1) {
        reloadingEndTime = SDL_GetTicks() + reloadingTime;
    }
}

#endif