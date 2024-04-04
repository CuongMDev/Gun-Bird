#ifndef GAME_H
#define GAME_H

#include "../AllObjects.h"
#include "Point.h"
#include "Item.h"

class Game
{
private:
    const int gameRoundsPerLevel = 10;

    Background *background;
    Ground *ground;
    MainBird *mainBird;
    ObjectsList *batList;
    Boss *boss;
    GameOver *gameOver;
    ObjectsList *pipeList;
    Point *point;
    Item *item;

    static void handleGameOver();
    void resetGame();
    void changeGamePaused();
    void addPoint(bool force = false);

    void upGameLevel();

    //check collision
    void checkCollisionObjects();
    void checkCollisionBatAndPlayerBullet();
    void checkCollisionBirdAndPipe();
    void checkCollisionPipeAndPlayerBullet();
    void checkCollisionObjectsBirdAndBat();
    void checkCollisionObjectsBirdAndItems();
    void checkCollisionObjectsBirdAndBoss();
    void checkCollisionObjectsBirdAndSrynge();
    void checkCollisionBossAndPlayerBullet();
    void checkGameOver();

public:
    Game();
    ~Game();

    void init();
    void handleEvent(SDL_Event *e);
    void handleKey(const Uint8 *currentKeyStates);
    void handleGameOverButtonClicked(BUTTON buttonClicked);
    void render();
};

Game::Game()
{
    Mix_AllocateChannels(static_cast<int>(GAME_CHANNEL::COUNT));

    pausedTime = 0;

    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
    batList = new ObjectsList();
    boss = new Boss();
    gameOver = new GameOver();
    pipeList = new ObjectsList();
    point = new Point;
    item = new Item();
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
    delete item;
}

void Game::handleGameOver()
{
    //stop scene move
    gVelocityYScene = 0;

    cursorMouse->setCursor(DEFAULT_CURSOR);
}

void Game::resetGame()
{
    init();

    pipeList->reset();
    Pipe::resetTime();

    batList->reset();
    Bat::resetTime();

    point->init();
    item->init();

    boss->init();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);
}

void Game::upGameLevel()
{
    gCurVelocityYScene += speedChangeWhenLevelChange;
    gVelocityYScene = gCurVelocityYScene;
}

void Game::init()
{
    gCurVelocityYScene = gInitVelocityYScene;
    gVelocityYScene = gInitVelocityYScene;
}

void Game::changeGamePaused()
{
    gamePaused = !gamePaused;

    if (gamePaused) {
        startPauseTime = SDL_GetTicks();

        cursorMouse->saveCursor();
        cursorMouse->setCursor(DEFAULT_CURSOR);
    }
    else {
        pausedTime += SDL_GetTicks() - startPauseTime;
        startPauseTime = -1;

        cursorMouse->loadSavedCursor();
    }
}

void Game::handleEvent(SDL_Event *e)
{
    mainBird->handleEvent(e);

    if (GameOver::gameIsOver()) {
        handleGameOverButtonClicked(gameOver->handleEvent(e));
    }
    else { //game not over
        if (e->type == SDL_KEYDOWN) {
            //Select surfaces based on key press
            switch (e->key.keysym.sym) {
                //pause game
                case SDLK_p:
                    changeGamePaused();
                    break;

                default:
                    break;
            }
        }
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
            //quit game
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
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
    checkCollisionObjects();
    checkGameOver();

    background->render();
    ground->render();
    Pipe::renderAll(pipeList);
    point->render();
    item->render(*pipeList);
    Bat::renderAll(batList);
    boss->render();
    if (!mainBird->render()) {
        gameOver->render();
    }
}

void Game::checkCollisionObjects()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    checkCollisionBatAndPlayerBullet();
    checkCollisionBirdAndPipe();
    checkCollisionPipeAndPlayerBullet();
    checkCollisionObjectsBirdAndBat();
    checkCollisionObjectsBirdAndItems();
    checkCollisionObjectsBirdAndBoss();
    checkCollisionObjectsBirdAndSrynge();
    checkCollisionBossAndPlayerBullet();
}

void Game::checkCollisionBatAndPlayerBullet()
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
                addPoint();
            }
        }
        else {
            //next bat
            objectA++;
        }

        continueToFind = true;
    }
}

void Game::checkCollisionBirdAndPipe()
{
    std::_List_iterator<Object *> object;
    if (pipeList->getCollisionObject(*mainBird, object)) {
        mainBird->changeHealth(-pipeDamage);
    }
}

void Game::checkCollisionPipeAndPlayerBullet()
{
    std::_List_iterator<Object *> objectA, objectB;
    auto &bulletList = mainBird->getBulletList();
    if (pipeList->getCollisionObjects(bulletList, objectA, objectB)) {
        bulletList.deleteObject(objectB);
    }
}

void Game::checkCollisionObjectsBirdAndBat()
{
    std::_List_iterator<Object *> object;
    bool continueToFind = false;

    //find live bat
    while (batList->getCollisionObject(*mainBird, object, continueToFind)) {
        Bat *bat = dynamic_cast<Bat*>(*object);
        if (!bat->isDied()) {
            mainBird->changeHealth(-batDamage);
            break;
        }
        //next bat
        object++;

        continueToFind = true;
    }
}

void Game::checkCollisionObjectsBirdAndItems()
{
    if (mainBird->checkCollisionObject(*item)) {
        if (item->isGunType()) {
            mainBird->addBulletCount(Item::toGunType(item->getItemType()), item->getItemValue());
        }
        else {
            //health item
            mainBird->changeHealth(item->getItemValue());
        }

        item->init();
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

void Game::checkCollisionObjectsBirdAndBoss()
{
    if (boss->isDied()) {
        return;
    }
    if (mainBird->checkCollisionObject(*boss)) {
        mainBird->changeHealth(-1);
    }
}

void Game::checkCollisionObjectsBirdAndSrynge()
{
    if (boss->isDied()) {
        return;
    }
    std::_List_iterator<Object *> object;
    if (boss->getSryngeList().getCollisionObject(*mainBird, object)) {
        mainBird->changeHealth(-1);
    }
}

void Game::checkCollisionBossAndPlayerBullet()
{
    if (boss->isDied()) {
        return;
    }
    std::_List_iterator<Object *> object;
    auto &bulletList = mainBird->getBulletList();
    if (bulletList.getCollisionObject(*boss, object)) {
        Bullets *bullet = dynamic_cast<Bullets*>(*object);
        boss->decreaseHealth(bullet->getDamage());
        if (boss->isDied()) {
            addPoint(true);
            if (boss->isDied()) {
                upGameLevel();
            }
        }

        bulletList.deleteObject(object);
    }
}

void Game::addPoint(bool force)
{
    if (force || point->getCurrentPoint() % gameRoundsPerLevel != gameRoundsPerLevel - 1) {
        point->addPoint();
        if (point->getCurrentPoint() % gameRoundsPerLevel == gameRoundsPerLevel - 1) {
            boss->continueInit();
        }
    }
}

#endif