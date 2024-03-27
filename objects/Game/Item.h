#ifndef ITEM_H
#define ITEM_H

enum ITEM_TYPE
{
    SILENT_PISTOL_ITEM,
    GOLD_PISTOL_ITEM,
    AK47_ITEM,
    WIN94_ITEM,
    SNIPER_ITEM,

    HEALTH_ITEM,

    ITEM_COUNT
};

class Item : public Object
{
private:
    LTexture *sTexture[ITEM_COUNT];
    static LTexture circleTexture;

    const std::vector<int> itemPercent = {15, 15, 15, 15, 15, 10};
    const std::vector<int> valuePercent = {57, 30, 10, 3};

    void setRandomTime(Uint32 time);
    void addRandomTime(Uint32 addTime);
    int randomTime;
    int mVelX;

    int itemValue;
    bool isRendering;

    ITEM_TYPE itemType;

    bool move();

    void loadIMG();

    void updateState(ObjectsList &pipeList);
    void renderTogRenderer() override;

public:
    Item();
    ~Item();

    void init();
    void render(ObjectsList &pipeList);

    void randomItem(ObjectsList &pipeList);
    ITEM_TYPE getItemType();
    int getItemValue() const;
    bool isGunType();

    static GUN_TYPE toGunType(const ITEM_TYPE &item);
    static ITEM_TYPE toItemType(const GUN_TYPE &gunType);
};

LTexture Item::circleTexture = {};

void Item::loadIMG()
{
    for (int item = SILENT_PISTOL_ITEM; item <= SNIPER_ITEM; item++) {
        sTexture[item] = Gun::getTexture(toGunType((ITEM_TYPE)item));
    }

    //https://steamcommunity.com/sharedfiles/filedetails/?id=1694204823
    sTexture[HEALTH_ITEM] = new LTexture();
    sTexture[HEALTH_ITEM]->loadFromFile(itemImagePath + "healthitem.png");
    circleTexture.loadFromFile(itemImagePath + "itemcircle.png");
}

bool Item::move()
{
    mPosX += mVelX;
    if (mPosX + mTexture->getWidth() < 0) {
        return false;
    }
    return true;
}

void Item::setRandomTime(Uint32 time)
{
    randomTime = time;
}

void Item::addRandomTime(Uint32 addTime)
{
    randomTime += addTime;
}

void Item::randomItem(ObjectsList &pipeList)
{
    itemType = getRandomWithPercent(itemPercent, std::vector<ITEM_TYPE>({SILENT_PISTOL_ITEM, GOLD_PISTOL_ITEM, AK47_ITEM, WIN94_ITEM, SNIPER_ITEM, HEALTH_ITEM}));
    mTexture = sTexture[itemType];
    mPosX = SCREEN_WIDTH;
    mPosY = getRandomNumber(circleTexture.getWidth() / 2, groundPosY - circleTexture.getWidth() / 2);

    //check collision with pipe
    std::_List_iterator<Object *> object;
    if (pipeList.getCollisionObject(*this, object)) {
        mPosY = groundPosY - mPosY;
    }

    itemValue = getRandomWithPercent<int>(valuePercent, {1, 2, 3, 4});
}

Item::Item() : Object(false)
{
    loadIMG();
    init();
}

Item::~Item()
{
    delete sTexture[HEALTH_ITEM];
}

void Item::init()
{
    setRandomTime(getCurrentTime() + 10000);
    mVelX = -gInitVelocityYScene;
    //to avoid collision with bird
    mPosX = SCREEN_WIDTH + 1;
    mPosY = 0;

    itemValue = 0;
    isRendering = false;
    //to avoid error when get dimension
    mTexture = sTexture[SILENT_PISTOL_ITEM];
}

void Item::updateState(ObjectsList &pipeList)
{
    if (!GameOver::gameIsOver()) {
        if (isRendering) {
            if (!move()) {
                init();
                return;
            }
        }
        else if (getCurrentTime() >= randomTime) {
            randomItem(pipeList);
            addRandomTime(10000);
            isRendering = true;
        }
    }
}

void Item::renderTogRenderer()
{
    if (isRendering) {
        circleTexture.render(mPosX + mTexture->getWidth() / 2 - circleTexture.getWidth() / 2, mPosY + mTexture->getHeight() / 2 - circleTexture.getHeight() / 2);
        mTexture->render(mPosX, mPosY);
    }
}

void Item::render(ObjectsList &pipeList)
{
    if (!gamePaused) {
        updateState(pipeList);
    }

    renderTogRenderer();
}

ITEM_TYPE Item::getItemType()
{
    return itemType;
}

int Item::getItemValue() const
{
    return itemValue;
}

bool Item::isGunType()
{
    return (SILENT_PISTOL_ITEM <= itemType && itemType <= SNIPER_ITEM);
}

GUN_TYPE Item::toGunType(const ITEM_TYPE &item)
{
    return (GUN_TYPE)(item - SILENT_PISTOL_ITEM + SILENT_PISTOL);
}

ITEM_TYPE Item::toItemType(const GUN_TYPE &gunType)
{
    return (ITEM_TYPE)(gunType - SILENT_PISTOL + SILENT_PISTOL_ITEM);
}

#endif //ITEM_H
