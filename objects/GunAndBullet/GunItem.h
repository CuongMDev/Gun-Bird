#ifndef GUNITEM_H
#define GUNITEM_H

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

class GunItem : public Object
{
private:
    const std::vector<int> magazinePercent = {57, 30, 10, 3};

    static LTexture circleTexture;

    void setRandomTime(Uint32 time);
    void addRandomTime(Uint32 addTime);
    int randomTime;
    int mVelX;

    int currentMagazineCount;
    bool isRendering;

    bool move();

protected:
    static const GunProperties gunProperties[GUN_COUNT];
    static LTexture sTexture[GUN_COUNT];
    GUN_TYPE currentGun;

    static void loadGunIMG();

    void updateState(ObjectsList &pipeList);
    void renderTogRenderer() override;

public:
    GunItem();

    void init();
    void render(ObjectsList &pipeList);

    void randomGun(ObjectsList &pipeList);
    GUN_TYPE getCurrentGunType();
    int getCurrentMagazineCount() const;
};

const GunProperties GunItem::gunProperties[] = {
        {3, 17, 1, 3, 15, 7, PISTOL_BULLET, PISTOL_CURSOR, "pistol.png"}, //pistol
        {3, 17, 1, 3, 10, 30, PISTOL_BULLET, PISTOL_CURSOR, "silentpistol.png"}, //silent pistol
        {20, 17,1,  5, 15, 1,GOLD_PISTOL_BULLET, GOLDPISTOL_CURSOR, "goldpistol.png"}, //gold pistol
        {5, 7, 1, 7, 5, 30,AK47_BULLET, AK47_CURSOR, "ak47.png"}, //AK47
        {7, 20, 10, 20, 20, 8,WIN94_BULLET, WIN94_CURSOR, "win94.png"}, //Win94
        {12, 30, 20, 20, 30, 5, SNIPER_BULLET, SNIPER_CURSOR, "sniper.png"}, //Sniper
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

void GunItem::randomGun(ObjectsList &pipeList)
{
    currentGun = static_cast<GUN_TYPE>(getRandomNumber(PISTOL + 1, GUN_COUNT - 1));
    mTexture = &sTexture[currentGun];
    mPosX = SCREEN_WIDTH;
    mPosY = getRandomNumber(circleTexture.getWidth() / 2, groundPosY - circleTexture.getWidth() / 2);

    //check collision with pipe
    std::_List_iterator<Object *> object;
    if (pipeList.getCollisionObject(*this, object)) {
        mPosY = groundPosY - mPosY;
    }

    currentMagazineCount = getRandomWithPercent<int>(magazinePercent, {1, 2, 3, 4});
}

GunItem::GunItem() : Object(false)
{
    loadGunIMG();
    init();
}

void GunItem::init()
{
    setRandomTime(getCurrentTime() + 10000);
    mVelX = -gInitVelocityYScene;
    //to avoid collision with bird
    mPosX = SCREEN_WIDTH + 1;
    mPosY = 0;

    currentMagazineCount = 0;
    isRendering = false;
    //to avoid error when get dimension
    mTexture = &sTexture[PISTOL];
}

void GunItem::updateState(ObjectsList &pipeList)
{
    if (!GameOver::gameIsOver()) {
        if (isRendering) {
            if (!move()) {
                init();
                return;
            }
        }
        else if (getCurrentTime() >= randomTime) {
            randomGun(pipeList);
            addRandomTime(10000);
            isRendering = true;
        }
    }
}

void GunItem::renderTogRenderer()
{
    if (isRendering) {
        circleTexture.render(mPosX + mTexture->getWidth() / 2 - circleTexture.getWidth() / 2, mPosY + mTexture->getHeight() / 2 - circleTexture.getHeight() / 2);
        mTexture->render(mPosX, mPosY);
    }
}

void GunItem::render(ObjectsList &pipeList)
{
    if (!gamePaused) {
        updateState(pipeList);
    }

    renderTogRenderer();
}

GUN_TYPE GunItem::getCurrentGunType()
{
    return currentGun;
}

int GunItem::getCurrentMagazineCount() const
{
    return currentMagazineCount;
}

#endif //GUNITEM_H
