#ifndef GAME_H
#define GAME_H

#include "objects.h"
#include "mainData.h"
#include "CursorMouse.h"

class Game
{
private:
    Background *background;
    Ground *ground;
    MainBird *mainBird;
    Bat *bat;
    GameOver *gameOver;

    //true if handled when game over;
    bool gameOverHandled;

    void init();
    void handleGameOver();
    void resetGame();

public:
    Game();
    ~Game();

    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    void handleGameOverButtonClicked(BUTTON buttonClicked);
    void render();
};

void Game::handleGameOver()
{
    if (gameOverHandled) {
        return;
    }
    gameOverHandled = true;

    //stop scene move
    gVelocityYScene = 0;

    cursorMouse->setCursor(DEFAULT_CURSOR);
}

void Game::resetGame()
{
    Pipe::reset();
    Bullets::reset();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);

    cursorMouse->setCursor(AIM_CURSOR);
    gameOverHandled = false;
}

Game::Game()
{
    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
    bat = new Bat(100, 100);
    gameOver = new GameOver();
    cursorMouse = new CursorMouse();
    resetGame();
}

Game::~Game()
= default;

void Game::handleEvent(SDL_Event *e)
{
    mainBird->handleEvent(e);

    if (GameOver::gameIsOver()) {
        handleGameOverButtonClicked(gameOver->handleEvent(e));
    }
}

void Game::handleKey(const Uint8 *currentKeyStates)
{
    mainBird->handleKey(currentKeyStates);
}

void Game::handleGameOverButtonClicked(BUTTON buttonClicked)
{
    switch (buttonClicked) {
        case HOMEBUTTON:
            //handle
            break;
        case RETRYBUTTON:
            resetGame();
            break;

        default: //no button clicked
            break;
    }
}

void Game::render()
{
    if (GameOver::gameIsOver()) {
        handleGameOver();
    }

    background->render();
    ground->render();
    bat->render();
    Pipe::renderAll();

    //bird is in ground
    if (!mainBird->render()) {
        gameOver->render();
    }
}
#endif