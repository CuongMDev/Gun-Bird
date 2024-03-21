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
    GunItem *gunItem;

    void handleGameOver();
    void resetGame();

    //check colision
    void checkColisionObjects();
    void checkColisionBirdAndEnemyBullet();
    void checkColisionBirdAndPipe();
    void checkColisionPipeAndPlayerBullet();
    void checkColisionObjectsBirdAndBat();
    void checkColisionObjectsBirdAndItems();
    void checkGameOver();

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
    gunItem = new GunItem();
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
    delete gunItem;
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
    Bat::resetTime();

    point->init();
    gunItem->init();

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
    checkGameOver();

    background->render();
    ground->render();
    Pipe::renderAll(pipeList);
    point->render();
    Bat::renderAll(batList);
    if (!mainBird->render()) {
        gameOver->render();
    }
    gunItem->render();
}

void Game::checkColisionObjects()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    checkColisionBirdAndEnemyBullet();
    checkColisionBirdAndPipe();
    checkColisionPipeAndPlayerBullet();
    checkColisionObjectsBirdAndBat();
    checkColisionObjectsBirdAndItems();
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
            Bullets *bullet = dynamic_cast<Bullets*>(*objectB);
            bat->decreaseHealth(bullet->getDamage());

            //delete bullet;
            objectB = bulletList.deleteObject(objectB);
            if (bat->isDied()) {
                //next bat
                objectA++;
                objectB = bulletList.getBegin();
                point->addPoint();
            }
        }
        else {
            //next bat
            objectA++;
        }

        continueToFind = true;
    }
}

void Game::checkColisionBirdAndPipe()
{
    std::_List_iterator<Object *> object;
    if (pipeList->getCollisionObject(*mainBird, object)) {
        mainBird->changeHealth(-30);
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

void Game::checkColisionObjectsBirdAndBat()
{
    std::_List_iterator<Object *> object;
    bool continueToFind = false;

    //find live bat
    while (batList->getCollisionObject(*mainBird, object, continueToFind)) {
        Bat *bat = dynamic_cast<Bat*>(*object);
        if (!bat->isDied()) {
            mainBird->changeHealth(-30);
            break;
        }
        //next bat
        object++;

        continueToFind = true;
    }
}

void Game::checkColisionObjectsBirdAndItems()
{
    if (mainBird->checkCollisionObject(*gunItem)) {
        mainBird->addBulletCount(gunItem->getCurrentGunType(), 30);
        gunItem->init();
    }
}


void Game::checkGameOver()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    if (mainBird->getCurrentHealth() == 0) {
        GameOver::onGameOver();
        handleGameOver();
    }
}

#endif