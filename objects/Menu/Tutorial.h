#ifndef TUTORIAL_H
#define TUTORIAL_H

class Tutorial
{
private:
    LTexture tutorialTexture;
    LTexture tutorialBackground;

    void loadIMG();
public:
    Tutorial();

    void render();
};

Tutorial::Tutorial()
{
    loadIMG();
}

void Tutorial::loadIMG()
{
    tutorialTexture.loadFromFile(tutorialImagePath + "tutorial.png");
    tutorialBackground.loadFromFile(tutorialImagePath + "background.png");
}

void Tutorial::render()
{
    tutorialBackground.render(SCREEN_WIDTH / 2 - tutorialBackground.getWidth() / 2, 130);
    tutorialTexture.render(SCREEN_WIDTH / 2 - tutorialTexture.getWidth() / 2, 190);
}


#endif //TUTORIAL_H
