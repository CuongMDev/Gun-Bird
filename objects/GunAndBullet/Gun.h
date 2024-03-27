#ifndef GUN_H
#define GUN_H

#include "Bullets.h"
#include "../CursorMouse/CursorMouse.h"
#include "ExplosionEffect.h"
#include "../Other/Font.h"
#include <cmath>
#include <list>

const int gunPosX = 5;
const int gunPosY = groundPosY + 10;

const int typePosX = 10;
const int typePosY = 90;

const int bulletTextPosX = 10;
const int bulletTextPosY = 120;

enum GUN_TYPE
{
    PISTOL,
    SILENT_PISTOL,
    GOLD_PISTOL,
    AK47,
    WIN94,
    SNIPER,

    GUN_COUNT
};

struct GunProperties;

class Gun : public Object
{
private:
    static const GunProperties gunProperties[GUN_COUNT];

    static LTexture sTexture[GUN_COUNT];
    GUN_TYPE currentGun;

    static const int reloadingTime;

    LTexture bulletLabel;

    int curBullet[GUN_COUNT];
    //-1 if the number of bullets is infinite
    int bulletLeft[GUN_COUNT];

    ObjectsList bulletsList;

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

    void loadIMG();

    void updateAngle();
    void addBullet();
    void calculateGunHeadPos(int &x, int &y);
    void restoreAim();
    void updateBulletText();
    //check and shoot
    void shoot();
    void checkReloading();
    void cancelReloading();
    //change gun to currentGun + val
    void switchGun(bool rightSwitch);
    bool checkGunStillHasBullets(GUN_TYPE gunType);
    bool updateState() override;
    void renderTogRenderer() override;

public:
    Gun();

    ObjectsList &getBulletList();

    void init();
    void handleEvent(SDL_Event *e);
    void setPosAndAngle(int x, int y);
    void setGun(GUN_TYPE gunType);
    void reload();
    void addBulletCount(GUN_TYPE gunType, int bulletMagazine);
    bool render() override;

    static LTexture *getTexture(const GUN_TYPE &gunType);
};

const int Gun::reloadingTime = 2000;

struct GunProperties
{
    int damage;
    int timeRestoreAim;
    int recoil;
    int maxVelRecoil;
    int shootDelay;
    int bulletInTape;

    BULLET_TYPE bulletType;

    CURSOR_TYPE cursorType;

    std::string imageName;
};

const GunProperties Gun::gunProperties[] = {
        {3, 17, 1, 3, 15, 7, PISTOL_BULLET, PISTOL_CURSOR, "pistol.png"}, //pistol
        {3, 17, 1, 3, 10, 30, PISTOL_BULLET, PISTOL_CURSOR, "silentpistol.png"}, //silent pistol
        {20, 17,1,  5, 15, 1,GOLD_PISTOL_BULLET, GOLDPISTOL_CURSOR, "goldpistol.png"}, //gold pistol
        {5, 7, 1, 7, 5, 30,AK47_BULLET, AK47_CURSOR, "ak47.png"}, //AK47
        {7, 20, 10, 20, 20, 8,WIN94_BULLET, WIN94_CURSOR, "win94.png"}, //Win94
        {12, 30, 20, 20, 30, 5, SNIPER_BULLET, SNIPER_CURSOR, "sniper.png"}, //Sniper
};
LTexture Gun::sTexture[];

Gun::Gun() : Object(false)
{
    loadIMG();
    init();
    updateBulletText();
}

ObjectsList &Gun::getBulletList()
{
    return bulletsList;
}

void Gun::loadIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    for (auto gunType = 0; gunType < GUN_COUNT; gunType++) {
        sTexture[gunType].loadFromFile(gunImagePath + gunProperties[gunType].imageName, true, 67, 76, 111);
    }
}

void Gun::init()
{
    mPosX = 0;
    mPosY = 0;
    mAngle = 0;
    mVelRecoil = 0;
    mouseHold = false;
    explosionEffect.init();

    curTime = 0;
    curTimeRestoreAim = 0;

    setGun(PISTOL);

    memset(curBullet, 0, sizeof(curBullet));
    memset(bulletLeft, 0, sizeof(bulletLeft));

    curBullet[currentGun] = gunProperties[currentGun].bulletInTape;
    bulletLeft[currentGun] = -1; //infinity
    updateBulletText();

    bulletsList.reset();
}

void Gun::updateAngle()
{
    mAngle = angleToMousePos(mPosX, mPosY);
}

void Gun::addBullet()
{
    int x, y;
    calculateGunHeadPos(x, y);

    //Adjust the bullet to fit into the barrel of the gun
    explosionEffect.start(x, y - 10, mAngle);
    bulletsList.add(new Bullets(x, y - 5, gunProperties[currentGun].bulletType, gunProperties[currentGun].damage));

    curBullet[currentGun]--;
    updateBulletText();
}

void Gun::calculateGunHeadPos(int &x, int &y)
{
    x = mPosX;
    y = mPosY + pivotY;
    calculateVelocityToMouse(x, y, getWidth());

    x += mPosX;
    y += mPosY + pivotY;
}

void Gun::restoreAim()
{
    if (curTimeRestoreAim > 0) {
        curTimeRestoreAim--;
        if (curTimeRestoreAim % gunProperties[currentGun].timeRestoreAim == 0) {
            cursorMouse->move(0, 10);
        }
    }

    if (curTimeRestoreAim % gunProperties[currentGun].timeRestoreAim == 0) {
        if (mVelRecoil > 0) {
            //restore gun aim
            mVelRecoil--;
        }
    }
}

void Gun::updateBulletText()
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

void Gun::shoot()
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
        cursorMouse->recoilMouse(1, 10);
    }
    else {
        cursorMouse->recoilMouse(1, 10 * mVelRecoil);
    }
}

void Gun::cancelReloading()
{
    reloadingEndTime = -1;
}

void Gun::switchGun(bool rightSwitch)
{
    int val = (rightSwitch) ? 1 : -1;
    int newGun = (int)currentGun + val;
    if (newGun < 0) newGun += GUN_COUNT;
    if (newGun >= GUN_COUNT) newGun -= GUN_COUNT;

    //find gun has bullets
    while (!checkGunStillHasBullets(static_cast<GUN_TYPE>(newGun))) {
        newGun += val;
        if (newGun < 0) newGun += GUN_COUNT;
        if (newGun >= GUN_COUNT) newGun -= GUN_COUNT;
    }

    setGun(static_cast<GUN_TYPE>(newGun));
}

bool Gun::checkGunStillHasBullets(GUN_TYPE gunType)
{
    if (bulletLeft[gunType] == -1) {
        return true;
    }
    if (curBullet[gunType] + bulletLeft[gunType] > 0) {
        return true;
    }

    return false;
}

void Gun::checkReloading()
{
    if (reloadingEndTime == -1) {
        return;
    }
    if (getCurrentTime() <= reloadingEndTime) {
        return;
    }

//------------------------------------------------
    //reset
    cancelReloading();

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


void Gun::handleEvent(SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            //reload
            case SDLK_r:
                reload();
                break;

            //switch gun
            case SDLK_q:
                switchGun(false);
                break;
            case SDLK_e:
                switchGun(true);
                break;

            //-------

            default:
                break;
        }
    }

    if (e->type == SDL_MOUSEBUTTONDOWN) {
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

bool Gun::updateState()
{
    updateAngle();
    checkReloading();
    shoot();

    return true;
}

void Gun::renderTogRenderer()
{
    //show type
    mTexture->render(typePosX, typePosY);
    //show bullet
    bulletLabel.render(bulletTextPosX, bulletTextPosY);

    bulletsList.renderAll();

    SDL_Point center{ pivotX, pivotY };
    mTexture->render(mPosX, mPosY, NULL, mAngle, &center);
    explosionEffect.render();
}

void Gun::setPosAndAngle(int x, int y)
{
    mPosX = x;
    mPosY = y;
}

void Gun::setGun(GUN_TYPE gunType)
{
    cancelReloading();
    currentGun = gunType;
    mTexture = &sTexture[currentGun];
    cursorMouse->setCursor(gunProperties[currentGun].cursorType);

    updateBulletText();
}

void Gun::reload()
{
    //no bullet
    if (bulletLeft[currentGun] == 0) {
        return;
    }
    if (reloadingEndTime == -1) {
        reloadingEndTime = getCurrentTime() + reloadingTime;
    }
}

void Gun::addBulletCount(GUN_TYPE gunType, int bulletMagazine)
{
    bulletLeft[gunType] += bulletMagazine * gunProperties[gunType].bulletInTape;
    updateBulletText();
}

bool Gun::render()
{
    return Object::render();
}

LTexture *Gun::getTexture(const GUN_TYPE &gunType)
{
    return &sTexture[gunType];
}

#endif