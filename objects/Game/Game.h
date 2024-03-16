#ifndef GAME_H
#define GAME_H

#include "../AllObjects.h"
#include "../CursorMouse/CursorMouse.h"
#include "Point.h"

class Game
{
private:
    Background *background;
    Ground *ground;
    MainBird *mainBird;
    ObjectsList *batList;
    GameOver *gameOver;
    ObjectsList *pipeList;
    Point *point;
    CursorMouse* cursorMouse;

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
    point = new Point;
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
    delete point;
    delete cursorMouse;
}

void Game::handleGameOver()
{
    //stop scene move
    gVelocityYScene = 0;

    CursorMouse::setCursor(DEFAULT_CURSOR);
}

void Game::resetGame()
{
    pipeList->reset();
    Pipe::resetTime();
    batList->reset();
    point->init();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);

    CursorMouse::setCursor(AIM_CURSOR);
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
    Pipe::renderAll(pipeList);
    point->render();
    Bat::renderAll(batList);
    if (!mainBird->render()) {
        gameOver->render();
    }
}

void Game::checkColisionObjects()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    checkColisionBirdAndEnemyBullet();
    checkColisionBirdAndPipe();
    checkColisionPipeAndPlayerBullet();
    //checkColisionObjectsBatAndPipe();
}

void Game::checkColisionBirdAndEnemyBullet()
{
    std::_List_iterator<Object *> objectA, objectB;
    auto &bulletList = mainBird->getBulletList();
    bool continueToFind = false;

    //find until cant find
    while (batList->getCollisionObjects(bulletList, objectA, objectB, continueToFind)) {
        Bat *bat = dynamic_cast<Bat*>(*objectA);

        if (!bat->isDied()) {
            bulletList.deleteObject(objectB);
            bat->decreaseHealth();
            if (bat->isDied()) {
                point->addPoint();
            }
        }
        else {
            //next bat
            objectB++;
        }

        continueToFind = true;
    }
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
    auto &bulletList = mainBird->getBulletList();
    if (pipeList->getCollisionObjects(bulletList, objectA, objectB)) {
        bulletList.deleteObject(objectB);
    }
}

void Game::checkColisionObjectsBatAndPipe()
{

}

#endif