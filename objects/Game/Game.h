#ifndef GAME_H
#define GAME_H

#include "../AllObjects.h"
#include "Score.h"
#include "Item.h"
#include "../Menu/Setting.h"

class Game
{
private:
    const int gameRoundsPerLevel = 5;

    const int scoreWhenHaveNoBoss = 100;
    const int scoreWhenHaveBoss = 5;

    int batCountNeedShootBeforeBoss;
    int bossScore;

    Background *background;
    Ground *ground;
    MainBird *mainBird;
    ObjectsList *batList;
    Boss *boss;
    GameOver *gameOver;
    ObjectsList *pipeList;
    Score *score;
    Item *item;

    void handleGameOver();
    void changeGamePaused();
    void addBatScore();
    void addBossScore();

    void upGameLevel();

    //check collision
    void checkCollisionObjects();
    void checkCollisionObjectBatAndPlayerBullet();
    void checkCollisionObjectBirdAndPipe();
    void checkCollisionObjectPipeAndPlayerBullet();
    void checkCollisionObjectsBirdAndBat();
    void checkCollisionObjectsBirdAndItems();
    void checkCollisionObjectsBirdAndBoss();
    void checkCollisionObjectsBirdAndSyringe();
    void checkCollisionObjectBossAndPlayerBullet();
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
    pausedTime = 0;

    background = new Background();
    ground = new Ground(groundPosX, groundPosY);
    mainBird = new MainBird(mainBirdPosX, mainBirdPosY);
    batList = new ObjectsList();
    boss = new Boss();
    gameOver = new GameOver();
    pipeList = new ObjectsList();
    score = new Score();
    item = new Item();
}

Game::~Game()
{
    delete background;
    delete ground;
    delete mainBird;
    delete batList;
    delete gameOver;
    delete pipeList;
    delete score;
    delete item;
}

void Game::handleGameOver()
{
    GameOver::onGameOver();
    //stop scene move
    gVelocityYScene = 0;

    cursorMouse->setCursor(DEFAULT_CURSOR);
}

void Game::upGameLevel()
{
    //recover health
    mainBird->changeMaxHealth(2);
    mainBird->setToMaxHealth();

    batCountNeedShootBeforeBoss = gameRoundsPerLevel - 1;

    gCurVelocityYScene += speedChangeWhenLevelChange;
    gVelocityYScene = gCurVelocityYScene;
}

void Game::init()
{
    Setting::allocateChannels(static_cast<int>(SOUND_CHANNEL_COUNT::GAME));

    batCountNeedShootBeforeBoss = gameRoundsPerLevel - 1;

    gCurVelocityYScene = gInitVelocityYScene;
    gVelocityYScene = gInitVelocityYScene;

    if (gamePaused) {
        changeGamePaused();
    }

    pipeList->reset();
    Pipe::resetTime();

    batList->reset();
    Bat::resetTime();

    score->init();
    item->init();

    boss->init();

    gVelocityYScene = gInitVelocityYScene;

    gameOver->reset();
    mainBird->init(mainBirdPosX, mainBirdPosY);
}

void Game::changeGamePaused()
{
    gamePaused = !gamePaused;

    if (gamePaused) {
        startPauseTime = SDL_GetTicks();

        cursorMouse->saveCursor();
        cursorMouse->setCursor(DEFAULT_CURSOR);
        score->startMoving(true);
    }
    else {
        pausedTime += SDL_GetTicks() - startPauseTime;
        startPauseTime = -1;
        score->startMoving(false);

        cursorMouse->loadSavedCursor();
    }
}

void Game::handleEvent(SDL_Event *e)
{
    if (GameOver::gameIsOver() || gamePaused) {
        handleGameOverButtonClicked(gameOver->handleEvent(e));
    }
    else {
        mainBird->handleEvent(e);
    }
    if (!GameOver::gameIsOver()) { //game not over
        if (e->type == SDL_KEYDOWN) {
            //Select surfaces based on key press
            switch (e->key.keysym.sym) {
                //pause game
                case SDLK_ESCAPE:
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
            gameStarted = -1;
            break;
        case RETRYBUTTON:
            init();
            break;
        case CONTINUE_BUTTON:
            changeGamePaused();
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
    item->render(*pipeList);
    Bat::renderAll(batList);
    boss->render();
    score->render();
    if (!mainBird->render() || gamePaused) {
        score->startMoving(true);
        gameOver->render();
    }
}

void Game::checkCollisionObjects()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    checkCollisionObjectBatAndPlayerBullet();
    checkCollisionObjectBirdAndPipe();
    checkCollisionObjectPipeAndPlayerBullet();
    checkCollisionObjectsBirdAndBat();
    checkCollisionObjectsBirdAndItems();
    checkCollisionObjectsBirdAndBoss();
    checkCollisionObjectsBirdAndSyringe();
    checkCollisionObjectBossAndPlayerBullet();
}

void Game::checkCollisionObjectBatAndPlayerBullet()
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
                addBatScore();
            }
        }
        else {
            //next bat
            objectA++;
        }

        continueToFind = true;
    }
}

void Game::checkCollisionObjectBirdAndPipe()
{
    std::_List_iterator<Object *> object;
    if (pipeList->getCollisionObject(*mainBird, object)) {
        mainBird->changeHealth(-pipeDamage);
    }
}

void Game::checkCollisionObjectPipeAndPlayerBullet()
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
            if (mainBird->changeHealth(-batDamage)) {
                bat->onDied();
            }
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
        else if (item->isHealth()) {
            //health item
            mainBird->changeHealth(item->getItemValue());
        }
        else {
            //shield
            mainBird->activeShield();
        }

        item->init(true);
    }
}


void Game::checkGameOver()
{
    if (GameOver::gameIsOver()) {
        return;
    }
    if (mainBird->getCurrentHealth() == 0) {
        handleGameOver();
    }
}

void Game::checkCollisionObjectsBirdAndBoss()
{
    if (boss->isDied()) {
        return;
    }
    if (mainBird->checkCollisionObject(*boss)) {
        if (mainBird->changeHealth(-1)) {
            bossScore /= 2;
        }
    }
}

void Game::checkCollisionObjectsBirdAndSyringe()
{
    if (boss->isDied()) {
        return;
    }
    std::_List_iterator<Object *> object;
    if (boss->getSyringeList().getCollisionObject(*mainBird, object)) {
        mainBird->changeHealth(-1);
        bossScore /= 2;
    }
}

void Game::checkCollisionObjectBossAndPlayerBullet()
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
            addBossScore();
            upGameLevel();
        }

        bulletList.deleteObject(object);
    }
}

void Game::addBatScore()
{
    if (batCountNeedShootBeforeBoss > 0) {
        score->addScore(scoreWhenHaveNoBoss);
        if (batCountNeedShootBeforeBoss > 0) {
            batCountNeedShootBeforeBoss--;
            if (!batCountNeedShootBeforeBoss) {
                boss->continueInit();
                bossScore = 2000;
            }
        }
    }
    else {
        //have boss
        score->addScore(scoreWhenHaveBoss);
    }
}

void Game::addBossScore()
{
    score->addScore(std::max(scoreWhenHaveNoBoss, bossScore));
}

#endif