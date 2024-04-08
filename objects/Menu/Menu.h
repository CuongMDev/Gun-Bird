#ifndef MENU_H
#define MENU_H

#include "../Other/Object.h"
#include "../Other/utils.h"
#include <vector>

enum OPTION
{
    PLAY,
    SETTING,
    TUTORIAL,
    EXIT,

    //NONE OR COUNT
    OPTION_COUNT
};

class Menu
{
private:
    std::vector<LTexture> backgroundTexture;
    const int imgCount = 4;
    const int imgChangeVel = 5;

    Game game;

    LTexture optionTexture[OPTION_COUNT];

    //The X and Y offsets
    int mPosX[OPTION_COUNT], mPosY[OPTION_COUNT];

    OPTION optionCollided;
    OPTION optionClicked;

    int curTimeRender;
    int curIMGRender;

    void init();
    void initText();
    void initPos();
    void loadIMG();
    void startGame();
    bool checkCollisionOption(OPTION option);

    bool updateState();
public:
    Menu();
    ~Menu();

    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);

    void render();
};

bool Menu::checkCollisionOption(OPTION option)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (checkCollision(mouseX, mouseY, 0, 0, mPosX[option], mPosY[option], optionTexture[option].getWidth(), optionTexture[option].getHeight())) {
        return true;
    }
    else {
        return false;
    }
}

void Menu::handleKey(const Uint8 *currentKeyStates)
{
    if (gameStarted) {
        game.handleKey(currentKeyStates);
    }
}

void Menu::handleEvent(SDL_Event *e)
{
    if (gameStarted) {
        game.handleEvent(e);
    }
    else {
        //none
        optionCollided = OPTION_COUNT;
        for (int option = PLAY; option < OPTION_COUNT; option++) {
            if (checkCollisionOption(static_cast<OPTION>(option))) {
                optionCollided = static_cast<OPTION>(option);
            }
        }

        //finished loading game over image or game paused
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            if (e->button.button == SDL_BUTTON_LEFT) {
                // left mouse button pressed
                optionClicked = optionCollided;
            }
        }
    }
}

Menu::Menu()
{
    init();
    loadIMG();
}

Menu::~Menu()
= default;

void Menu::init()
{
    curIMGRender = 0;
    curTimeRender = 0;

    optionCollided = OPTION_COUNT;
    optionClicked = OPTION_COUNT;

    initText();
    initPos();
}

void Menu::initText()
{
    static SDL_Color textColor = { 245, 117, 66, 255 };

    optionTexture[PLAY].loadFromRenderedText("Play", mightySoulyFonts[FontStyle::Regular], textColor);
    optionTexture[SETTING].loadFromRenderedText("Setting", mightySoulyFonts[FontStyle::Regular], textColor);
    optionTexture[TUTORIAL].loadFromRenderedText("Tutorial", mightySoulyFonts[FontStyle::Regular], textColor);
    optionTexture[EXIT].loadFromRenderedText("Exit", mightySoulyFonts[FontStyle::Regular], textColor);
}

void Menu::initPos()
{
    mPosX[PLAY] = SCREEN_WIDTH / 2 - optionTexture[PLAY].getWidth() / 2;
    mPosY[PLAY] = 270;

    mPosX[SETTING] = SCREEN_WIDTH / 2 - optionTexture[SETTING].getWidth() / 2;
    mPosY[SETTING] = mPosY[PLAY] + 50;

    mPosX[TUTORIAL] = SCREEN_WIDTH / 2 - optionTexture[TUTORIAL].getWidth() / 2;
    mPosY[TUTORIAL] = mPosY[SETTING] + 50;

    mPosX[EXIT] = SCREEN_WIDTH / 2 - optionTexture[EXIT].getWidth() / 2;
    mPosY[EXIT] = mPosY[TUTORIAL] + 50;
}

void Menu::startGame()
{
    gameStarted = true;
    game.init();
}

void Menu::loadIMG()
{
    backgroundTexture.resize(imgCount);
    for (int i = 0; i < imgCount; i++) {
        backgroundTexture[i].loadFromFile(menuBackgroundImagePath + "menubackground" + std::to_string(i) + ".png");
    }
}

bool Menu::updateState()
{
    curTimeRender++;
    if (curTimeRender >= imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender >= imgCount) {
            curIMGRender = 0;
        }
    }

    return true;
}

void Menu::render()
{
    switch (optionClicked) {
        case PLAY:
            if (gameStarted == 0) {
                startGame();
            }
            game.render();
            if (gameStarted == -1) {
                //back to menu
                optionClicked = OPTION_COUNT;
                //reset
                gameStarted = 0;
            }
            break;
        case SETTING:
            break;
        case TUTORIAL:
            break;
        case EXIT:
            //quit
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
            break;

        default:
            //show menu
            updateState();
            backgroundTexture[curIMGRender].render(0, 0);
            for (int option = PLAY; option < OPTION_COUNT; option++) {
                if (option == optionCollided) {
                    optionTexture[option].render(mPosX[option], mPosY[option], NULL, 0, NULL, SDL_FLIP_NONE, 30);
                }
                else {
                    optionTexture[option].render(mPosX[option], mPosY[option]);
                }
            }
            break;
    }
}

#endif //MENU_H
