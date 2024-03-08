#ifndef GAME_H
#define GAME_H

#include "objects.h"
#include "mainData.h"

class Game
{
private:
    Background *background;
    Ground *ground;
    MainBird *mainBird;
    GameOver *gameOver;

    bool checkOutTheBorder();

    void init();
    void handleGameOver();
    void resetGame();
    void renderPipe(std::list<Pipe *> &pipeList);

public:
    Game();
    ~Game();

    void handleEvent(SDL_Event e);
    void handleKey(const Uint8 *currentKeyStates);
    void render();
};

void Game::handleGameOver()
{
    //stop scene move
    gVelocityYScene = 0;
}

void Game::resetGame()
{
    Pipe::reset();
    Bullets::reset();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);
}

Game::Game()
{
    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
    gameOver = new GameOver();
}

Game::~Game()
{
}

void Game::handleEvent(SDL_Event e)
{
    if (e.key.keysym.sym == SDLK_LSHIFT) {
        resetGame();
    }
    mainBird->handleEvent(e);
}

void Game::handleKey(const Uint8 *currentKeyStates)
{
    mainBird->handleKey(currentKeyStates);
}

void Game::render()
{
    if (GameOver::gameIsOver()) {
        handleGameOver();
    }

    background->render();
    ground->render();
    Pipe::renderAll();

    //bird is in ground
    if (!mainBird->render()) {
        gameOver->render();
    }
}

#endif