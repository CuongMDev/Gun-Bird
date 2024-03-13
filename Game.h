#ifndef GAME_H
#define GAME_H

#include "allObjects.h"
#include "mainData.h"
#include "CursorMouse.h"

class Game
{
private:
    Background *background;
    Ground *ground;
    MainBird *mainBird;
    ObjectsList *batList;
    GameOver *gameOver;
    ObjectsList *pipeList;

    void handleGameOver();
    void resetGame();

    //check colision
    void checkColisionObjects();
    void checkColisionBirdAndEnemyBullet();
    void checkColisionBirdAndPipe();
    void checkColisionPipeAndPlayerBullet();
    void checkColisionObjectsBatAndPipe();

public:
    Game();
    ~Game();

    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    void handleGameOverButtonClicked(BUTTON buttonClicked);
    void render();
};

Game::Game()
{
    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
    batList = new ObjectsList();
    gameOver = new GameOver();
    pipeList = new ObjectsList();
    cursorMouse = new CursorMouse();
    resetGame();
}

Game::~Game()
{
    delete background;
    delete ground;
    delete mainBird;
    delete batList;
    delete gameOver;
    delete pipeList;
}

void Game::handleGameOver()
{
    //stop scene move
    gVelocityYScene = 0;

    cursorMouse->setCursor(DEFAULT_CURSOR);
}

void Game::resetGame()
{
    pipeList->reset();
    batList->reset();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);

    cursorMouse->setCursor(AIM_CURSOR);
}

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
    checkColisionObjects();

    background->render();
    ground->render();
    Bat::renderAll(batList);
    Pipe::renderAll(pipeList);
    if (!mainBird->render()) {
        gameOver->render();
    }
}

void Game::checkColisionObjects()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    //checkColisionBirdAndEnemyBullet();
    checkColisionBirdAndPipe();
    checkColisionPipeAndPlayerBullet();
    //checkColisionObjectsBatAndPipe();
}

void Game::checkColisionBirdAndEnemyBullet()
{
    Object object;
//    if (mainBird->render())
}

void Game::checkColisionBirdAndPipe()
{
    std::_List_iterator<Object *> object;
    if (pipeList->getCollisionObject(*mainBird, object)) {
        GameOver::onGameOver();
        handleGameOver();
    }
}

void Game::checkColisionPipeAndPlayerBullet()
{
    std::_List_iterator<Object *> objectA, objectB;
    auto bulletList = mainBird->getBulletList();
    if (pipeList->getCollisionObjects(mainBird->getBulletList(), objectA, objectB)) {
        bulletList.deleteObject(objectB);
    }
}

void Game::checkColisionObjectsBatAndPipe()
{

}

#endif