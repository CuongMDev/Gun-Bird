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

    bool checkOutTheBorder();

    void init();
    void handleGameOver();
    void resetGame();
    void renderPipe(std::list<Pipe *> &pipeList);

public:
    Game();
    ~Game();

    void handleEvent(SDL_Event e);
    void render();
};

void Game::handleGameOver()
{
    //stop scene move
    gVelocityYScene = 0;
}

void Game::resetGame()
{
    gameOver = false;
    Pipe::reset();
    Bullets::reset();

    gVelocityYScene = gInitVelocityYScene;

    mainBird->init(mainBirdPosX, mainBirdPosY);
}

Game::Game()
{
    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
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

void Game::render()
{
    if (gameOver) {
        handleGameOver();
    }

    background->render();
    ground->render();
    Pipe::renderAll();

    mainBird->render();
}

#endif