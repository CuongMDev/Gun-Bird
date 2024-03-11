#ifndef BAT_H
#define BAT_H

#include "Character.h"

class Bat
{
private:
    Character *mCharacter;

public:
    Bat(int x, int y);
    ~Bat();

    void init(int x, int y);
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);

    bool render();
};

Bat::Bat(int x, int y)
{
    mCharacter = new Character(x, y, BAT);
    init(x, y);
}

Bat::~Bat()
= default;

void Bat::init(int x, int y)
{
    mCharacter->init(x, y);
}

bool Bat::render()
{
    bool rendered = mCharacter->render();

    if (!mCharacter->isDied()) {
    }

    return rendered;
}


#endif //BAT_H
