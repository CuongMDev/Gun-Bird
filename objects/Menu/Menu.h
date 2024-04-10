#ifndef MENU_H
#define MENU_H

#include "../Other/Object.h"
#include "../Other/utils.h"
#include <vector>
#include "Tutorial.h"
#include "Setting.h"

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

    Mix_Music *introSound;

    Game game;

    LTexture optionTexture[OPTION_COUNT];
    LTexture gameTitleText;

    Tutorial tutorial;
    Setting setting;

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
    void loadSound();
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
    if (cursorMouse->checkMouseCollision(mPosX[option], mPosY[option], optionTexture[option].getWidth(), optionTexture[option].getHeight())) {
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
        if (optionClicked == OPTION_COUNT) {
            //none
            optionCollided = OPTION_COUNT;
            for (int option = PLAY; option < OPTION_COUNT; option++) {
                if (checkCollisionOption(static_cast<OPTION>(option))) {
                    optionCollided = static_cast<OPTION>(option);
                    break;
                }
            }

            //finished loading game over image or game paused
            if (e->type == SDL_MOUSEBUTTONDOWN) {
                if (e->button.button == SDL_BUTTON_LEFT) {
                    // left mouse button pressed
                    optionClicked = optionCollided;
                    if (optionClicked != OPTION_COUNT) {
                        GameOver::playButtonSound();
                    }
                }
            }
        }
        else {
            if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE) { //back to menu
                //reset;
                optionClicked = OPTION_COUNT;
            }
            if (optionClicked == SETTING) {
                setting.handleEvent(e);
            }
        }
    }
}

Menu::Menu()
{
    loadIMG();
    loadSound();
    init();
}

Menu::~Menu()
{
    Mix_FreeMusic(introSound);
}

void Menu::init()
{
    gameStarted = 0;

    curIMGRender = 0;
    curTimeRender = 0;

    optionCollided = OPTION_COUNT;
    optionClicked = OPTION_COUNT;

    Setting::allocateChannels(static_cast<int>(SOUND_CHANNEL_COUNT::MENU));

    Mix_PlayMusic(introSound, -1);

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
    Mix_HaltMusic();
    game.init();
}

void Menu::loadIMG()
{
    backgroundTexture.resize(imgCount);
    for (int i = 0; i < imgCount; i++) {
        backgroundTexture[i].loadFromFile(menuBackgroundImagePath + "menubackground" + std::to_string(i) + ".png");
    }

    gameTitleText.loadFromFile(menuBackgroundImagePath + "gameTitle.png");
}

void Menu::loadSound()
{
    introSound = Mix_LoadMUS((introSoundPath + "intro.wav").c_str());
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
    if (optionClicked == PLAY) {
        if (gameStarted == 0) {
            startGame();
        }
        game.render();
        if (gameStarted == -1) {
            //back to menu
            init();
        }
    }
    else {
        //show background
        updateState();
        backgroundTexture[curIMGRender].render(0, 0);
        gameTitleText.render(260, 120);
        switch (optionClicked) {
            case SETTING:
                setting.render();
                break;
            case TUTORIAL:
                tutorial.render();
                break;
            case EXIT:
                //quit
                SDL_Event quitEvent;
                quitEvent.type = SDL_QUIT;
                SDL_PushEvent(&quitEvent);
                break;

            default:
                //show menu
                for (int option = PLAY; option < OPTION_COUNT; option++) {
                    if (option == optionCollided) {
                        optionTexture[option].render(mPosX[option], mPosY[option], NULL, 0, NULL, SDL_FLIP_NONE, 30);
                    } else {
                        optionTexture[option].render(mPosX[option], mPosY[option]);
                    }
                }
                break;
        }
    }
}

#endif //MENU_H
