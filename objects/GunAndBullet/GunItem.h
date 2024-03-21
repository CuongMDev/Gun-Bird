#ifndef GUNITEM_H
#define GUNITEM_H

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

class GunItem : public Object
{
private:
    static LTexture circleTexture;

    void setRandomTime(Uint32 time);
    void addRandomTime(Uint32 addTime);
    int randomTime;
    int mVelX;

    bool isRendering;

    bool move();

protected:
    static const GunProperties gunProperties[GUN_COUNT];
    static LTexture sTexture[GUN_COUNT];
    GUN_TYPE currentGun;

    static void loadGunIMG();

public:
    GunItem();

    void init();
    bool render() override;

    void randomGun();
    GUN_TYPE getCurrentGunType();
};

const GunProperties GunItem::gunProperties[] = {
        {3, 17, 1, 3, 15, 7, PISTOL_BULLET, "pistol.png"}, //pistol
        {3, 17, 1, 3, 10, 7, PISTOL_BULLET, "silentpistol.png"}, //silent pistol
        {20, 17,1,  5, 15, 1,GOLD_PISTOL_BULLET, "goldpistol.png"}, //gold pistol
        {5, 7, 1, 7, 5, 30,AK47_BULLET, "ak47.png"}, //AK47
        {12, 30, 20, 20, 30, 5, SNIPER_BULLET, "sniper.png"}, //Sniper
};

LTexture GunItem::circleTexture = {};
LTexture GunItem::sTexture[];

void GunItem::loadGunIMG()
{
    static bool loadedIMG = false;
    if (loadedIMG) {
        return;
    }
    loadedIMG = true;

    //image: https://midnitepixelated.itch.io/pixel-guns
    for (auto gunType = 0; gunType < GUN_COUNT; gunType++) {
        sTexture[gunType].loadFromFile(gunImagePath + gunProperties[gunType].imageName, true, 67, 76, 111);
    }
    circleTexture.loadFromFile(gunImagePath + "itemcircle.png");
}

bool GunItem::move()
{
    mPosX += mVelX;
    if (mPosX + mTexture->getWidth() < 0) {
        return false;
    }
    return true;
}

void GunItem::setRandomTime(Uint32 time)
{
    randomTime = time;
}

void GunItem::addRandomTime(Uint32 addTime)
{
    randomTime += addTime;
}

void GunItem::randomGun()
{
    currentGun = static_cast<GUN_TYPE>(getRandomNumber(PISTOL + 1, GUN_COUNT - 1));
    mTexture = &sTexture[currentGun];
    mPosX = SCREEN_WIDTH;
    mPosY = getRandomNumber(0, groundPosY - circleTexture.getWidth());
}

GunItem::GunItem() : Object(false)
{
    loadGunIMG();
    init();
}

void GunItem::init()
{
    setRandomTime(SDL_GetTicks() + 10000);
    mVelX = -gInitVelocityYScene;
    //to avoid collision with bird
    mPosX = SCREEN_WIDTH + 1;
    mPosY = 0;
    isRendering = false;
    //to avoid error when get dimension
    mTexture = &sTexture[PISTOL];
}

bool GunItem::render()
{
    if (!GameOver::gameIsOver()) {
        if (isRendering) {
            if (!move()) {
                init();
                return false;
            }
        } else if (SDL_GetTicks() >= randomTime) {
            randomGun();
            addRandomTime(10000);
            isRendering = true;
        }
    }

    if (isRendering) {
        circleTexture.render(mPosX, mPosY);
        mTexture->render(mPosX + circleTexture.getWidth() / 2 - mTexture->getWidth() / 2, mPosY + circleTexture.getHeight() / 2 - mTexture->getHeight() / 2);
        return true;
    }
    return false;
}

GUN_TYPE GunItem::getCurrentGunType()
{
    return currentGun;
}

#endif //GUNITEM_H