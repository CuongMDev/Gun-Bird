#ifndef GUN_H
#define GUN_H

#include "Bullets.h"
#include "../CursorMouse/CursorMouse.h"
#include "ExplosionEffect.h"
#include "../Other/Font.h"
#include <cmath>
#include <list>
#include "../Other/SoundChannel.h"

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
    enum GUN_SOUND_TYPE
    {
        SHOOT_SOUND,
        RELOAD_SOUND,
        PULL_OUT_SOUND,

        GUN_SOUND_COUNT
    };

    static const GunProperties gunProperties[GUN_COUNT];

    std::vector<Mix_Chunk*> gunSound[GUN_COUNT][3];
    static const std::string soundName[GUN_SOUND_COUNT];
    const int pullOutSoundTime = 17;
    const int reloadSoundTime = 37;

    static LTexture sTexture[GUN_COUNT];
    GUN_TYPE currentGun;

    LTexture bulletLabel;

    int curBullet[GUN_COUNT];
    //-1 if the number of bullets is infinite
    int bulletLeft[GUN_COUNT];

    ObjectsList bulletsList;

    ExplosionEffect explosionEffect;

    //The angle
    double mAngle;

    //shoot when curShootTime=shootDelay
    int curShootTime;

    int curTimeRestoreAim;
    int mVelRecoil;

    bool mouseHold;

    int curSoundTime;

    //=SoundCount if it is not playing
    GUN_SOUND_TYPE curPlayingSound;

    void loadIMG();
    void loadSound();

    void updateAngle();
    void addBullet();
    void calculateGunHeadPos(int &x, int &y, const int &width);
    void restoreAim();
    void updateBulletText();
    void checkShoot();
    void shoot();
    void cancelReloading();
    bool checkReloading();
    //change gun to currentGun + val
    void switchGun(bool rightSwitch);
    bool checkGunStillHasBullets(GUN_TYPE gunType);

    void setCurSound(GUN_SOUND_TYPE soundType = GUN_SOUND_COUNT);

    void activeShootSound();
    void activePullOutSound();
    void activeReloadSound();
    void playingSound();

    bool updateState() override;
    void renderTogRenderer() override;

public:
    Gun();
    ~Gun();

    ObjectsList &getBulletList();

    void init();
    void handleEvent(SDL_Event *e);
    void setPosAndAngle(int x, int y);
    void setGun(GUN_TYPE gunType);
    //true if reloading finished
    void reload(bool finished = false);
    void addBulletCount(GUN_TYPE gunType, int bulletMagazine);

    static LTexture *getTexture(const GUN_TYPE &gunType);
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

    CURSOR_TYPE cursorType;

    std::string name;
};

const GunProperties Gun::gunProperties[] = {
        {3, 17, 1, 3, 15, 7, PISTOL_BULLET, PISTOL_CURSOR, "pistol"}, //pistol
        {3, 17, 1, 3, 10, 30, PISTOL_BULLET, PISTOL_CURSOR, "silentpistol"}, //silent pistol
        {30, 17,1,  5, 15, 1,GOLD_PISTOL_BULLET, GOLDPISTOL_CURSOR, "goldpistol"}, //gold pistol
        {5, 7, 1, 7, 7, 30,AK47_BULLET, AK47_CURSOR, "ak47"}, //AK47
        {7, 20, 10, 20, 20, 8,WIN94_BULLET, WIN94_CURSOR, "win94"}, //Win94
        {12, 50, 20, 20, 50, 5, SNIPER_BULLET, SNIPER_CURSOR, "sniper"}, //Sniper
};
const std::string Gun::soundName[] = {
        {"0"}, //shoot
        {"reload"}, //reload
        {"pullout"}, //pullout
};

LTexture Gun::sTexture[];

Gun::Gun() : Object(false)
{
    loadIMG();
    loadSound();
    init();
    updateBulletText();
}

Gun::~Gun()
{
    for (auto & gunType : gunSound) {
        for (auto & soundType : gunType) {
            for (auto &i : soundType) {
                Mix_FreeChunk(i);
            }
        }
    }
}


ObjectsList &Gun::getBulletList()
{
    return bulletsList;
}

void Gun::loadIMG()
{
    //image: https://midnitepixelated.itch.io/pixel-guns
    for (auto gunType = 0; gunType < GUN_COUNT; gunType++) {
        sTexture[gunType].loadFromFile(gunImagePath + gunProperties[gunType].name + ".png", true, 67, 76, 111);
    }
}

void Gun::loadSound()
{
    for (auto gunType = 0; gunType < GUN_COUNT; gunType++) {
        for (auto soundType = 0; soundType < GUN_SOUND_COUNT; soundType++) {
            for (int count = 1;; count++) {
                std::string path = gunSoundPath + gunProperties[gunType].name + '/' + gunProperties[gunType].name + '_' + soundName[soundType] + std::to_string(count) + ".wav";
                Mix_Chunk *mixChunk = Mix_LoadWAV(path.c_str());

                if (mixChunk == NULL) {
                    delete mixChunk;
                    break;
                }
                gunSound[gunType][soundType].push_back(mixChunk);
            }
        }
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

    curShootTime = 0;
    curTimeRestoreAim = 0;

    //to make set gun not be ignored
    currentGun = GUN_COUNT;
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
    mAngle = cursorMouse->angleToMousePos(mPosX, mPosY);
}

void Gun::addBullet()
{
    int x, y;
    calculateGunHeadPos(x, y, getWidth());
    //Adjust the bullet to fit into the barrel of the gun
    explosionEffect.start(x, y - 8, mAngle);

    calculateGunHeadPos(x, y, getWidth() - 20);
    //Adjust the bullet to fit into the barrel of the gun
    bulletsList.add(new Bullets(x + 5, y - 8, gunProperties[currentGun].bulletType, gunProperties[currentGun].damage));

    curBullet[currentGun]--;
    updateBulletText();
}

void Gun::calculateGunHeadPos(int &x, int &y, const int &width)
{
    x = mPosX;
    y = mPosY + pivotY;
    cursorMouse->calculateVelocityToMouse(x, y, width);

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

void Gun::checkShoot() {
    if (curPlayingSound != SHOOT_SOUND && curPlayingSound != GUN_SOUND_COUNT) {
        return;
    }
    //no bullet
    if (curBullet[currentGun] == 0) {
        if (!mouseHold) {
            reload();
        }
        return;
    }
    //is reloading
    if (checkReloading()) {
        return;
    }
    //increase cur shoot time
    if (curShootTime < gunProperties[currentGun].shootDelay) {
        curShootTime++;
    }

    //check mouse hold
    if (!mouseHold) {
        restoreAim();
        return;
    }

    if (curShootTime < gunProperties[currentGun].shootDelay) {
        //It's not time to shoot yet
        return;
    }

    shoot();
}

void Gun::shoot()
{
    addBullet();
    activeShootSound();

    //increase time restore aim
    if (curTimeRestoreAim < gunProperties[currentGun].timeRestoreAim * 2) {
        curTimeRestoreAim += gunProperties[currentGun].timeRestoreAim;
    }

    mVelRecoil = std::min(mVelRecoil + gunProperties[currentGun].recoil, gunProperties[currentGun].maxVelRecoil);
    //reset
    curShootTime = 0;
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
    setCurSound();
}

bool Gun::checkReloading()
{
    return (curPlayingSound == RELOAD_SOUND);
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

void Gun::setCurSound(Gun::GUN_SOUND_TYPE soundType)
{
    //reset;
    curSoundTime = 0;
    curPlayingSound = soundType;
}

void Gun::activeShootSound()
{
    static int currentShoot = 0;

    int soundCount = (int)gunSound[currentGun][SHOOT_SOUND].size();
    currentShoot++;
    if (currentShoot >= soundCount) {
        currentShoot = 0;
    }

    int soundChannel = static_cast<int>(GUN_SOUND_CHANNEL::GUN_SHOOT_1) + currentShoot;
    Mix_PlayChannel(soundChannel, gunSound[currentGun][SHOOT_SOUND][getRandomNumber(0, soundCount - 1)], 0);
    setCurSound();
}

void Gun::activePullOutSound()
{
    int soundCount = (int)gunSound[currentGun][PULL_OUT_SOUND].size();
    int soundChannel = static_cast<int>(GUN_SOUND_CHANNEL::GUN_PULL_OUT_AND_RELOAD);

    if (curSoundTime > (soundCount - 1) * pullOutSoundTime) {
        //finished pull out
        if (!Mix_Playing(soundChannel)) {
            setCurSound();
        }
        return;
    }

    if (curSoundTime % pullOutSoundTime == 0) {
        Mix_PlayChannel(soundChannel, gunSound[currentGun][PULL_OUT_SOUND][curSoundTime / pullOutSoundTime], 0);
    }
    curSoundTime++;
}

void Gun::activeReloadSound()
{
    int soundCount = (int)gunSound[currentGun][RELOAD_SOUND].size();
    int soundChannel = static_cast<int>(GUN_SOUND_CHANNEL::GUN_PULL_OUT_AND_RELOAD);

    if (curSoundTime > (soundCount - 1) * reloadSoundTime) {
        //finished reloading
        if (!Mix_Playing(soundChannel)) {
            reload(true);
            setCurSound(PULL_OUT_SOUND);
        }
        return;
    }
    if (curSoundTime % reloadSoundTime == 0) {
        Mix_PlayChannel(soundChannel, gunSound[currentGun][RELOAD_SOUND][curSoundTime / reloadSoundTime], 0);
    }
    curSoundTime++;
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
    checkShoot();
    playingSound();

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
    if (currentGun == gunType) {
        return;
    }

    cancelReloading();
    currentGun = gunType;
    mTexture = &sTexture[currentGun];
    cursorMouse->setCursor(gunProperties[currentGun].cursorType);

    updateBulletText();
    reload();

    setCurSound(PULL_OUT_SOUND);
}

void Gun::reload(bool finished)
{
    if (!finished) {
        //full bullet
        if (curBullet[currentGun] == gunProperties[currentGun].bulletInTape) {
            return;
        }
        //no bullet
        if (bulletLeft[currentGun] == 0) {
            return;
        }
        if (!checkReloading()) {
            setCurSound(RELOAD_SOUND);
        }
    }
    else { //finished
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
}

void Gun::addBulletCount(GUN_TYPE gunType, int bulletMagazine)
{
    bulletLeft[gunType] += bulletMagazine * gunProperties[gunType].bulletInTape;
    updateBulletText();
}

LTexture *Gun::getTexture(const GUN_TYPE &gunType)
{
    return &sTexture[gunType];
}

void Gun::playingSound()
{
    switch (curPlayingSound) {
        case SHOOT_SOUND:
            activeShootSound();
            break;
        case PULL_OUT_SOUND:
            activePullOutSound();
            break;
        case RELOAD_SOUND:
            activeReloadSound();
            break;

        default:
            break;
    }
}

#endif