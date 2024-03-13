#ifndef BAT_H
#define BAT_H

#include "Character.h"
#include "ObjectsList.h"

const int batPosX = SCREEN_WIDTH;
const int batMinHeight = 50;
const int batSpeed = 1;

class Bat : public Character
{
private:
    static bool onAdd;

public:
    Bat(int x, int y);
    ~Bat();

    static void spawnBat(ObjectsList *batList);
    static void renderAll(ObjectsList *batList);

    void init(int x, int y);

    bool render() override;
};

Bat::Bat(int x, int y) : Character(x, y, BAT)
{
    init(x, y);
}

Bat::~Bat()
= default;

void Bat::init(int x, int y)
{
    initCharacter(x, y);
}

bool Bat::render()
{
    bool rendered = renderCharacter();

    if (!isDied()) {
        //go in screen
        if (mPosX + getWidth() > SCREEN_WIDTH - 50) {
            mPosX -= batSpeed;
        }
    }

    return rendered;
}

bool Bat::onAdd = false;

void Bat::spawnBat(ObjectsList *batList)
{
    if (SDL_GetTicks() >= nextCreatedTime) {
        //bat Y
        int batPosY = getRandomNumber( batMinHeight, SCREEN_HEIGHT);
        //create new bat
        Bat *bat = new Bat(batPosX, batPosY);
        batList->add(bat);
    }
}

void Bat::renderAll(ObjectsList *batList)
{
    if (!GameOver::gameIsOver()) {
        spawnBat(batList);
    }

    batList->renderAll();
}

#endif //BAT_H
