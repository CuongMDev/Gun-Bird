#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <fstream>
#include <filesystem>

class HighScores
{
private:
    LTexture backTexture;
    LTexture highScoresBackground;

    static const int scoresCount = 10;

    static std::pair<int, tm> highScores[scoresCount];

    void init();
    void loadIMG();
    static void loadHighScores();
public:
    HighScores();

    static void addScores(int score);
    void render();
};

std::pair<int, tm> HighScores::highScores[scoresCount] = {};

HighScores::HighScores()
{
    init();
    loadIMG();
}

void HighScores::init()
{
    memset(highScores, 0, sizeof(highScores));
    loadHighScores();
}

void HighScores::loadIMG()
{
    backTexture.loadFromFile(highScoresImagePath + "text.png");
    highScoresBackground.loadFromFile(highScoresImagePath + "background.png");
}

void HighScores::loadHighScores()
{
    std::ifstream inFile(highScoresPath, std::ios::binary);
    if (!inFile.is_open()) {
        return;
    }

    inFile.read(reinterpret_cast<char*>(&highScores), sizeof(highScores));
    inFile.close();
}

void HighScores::addScores(int score)
{
    if (score == 0) {
        return;
    }

    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directory(dataPath);
    }
    std::ofstream outFile(highScoresPath, std::ios::binary);
    if (!outFile.is_open()) {
        return;
    }

    //add
    for (int i = 0; i < scoresCount; i++) {
        if (highScores[i].first < score) {
            for (int j = scoresCount - 1; j > i; j--) {
                highScores[j] = highScores[j - 1];
            }

            time_t now = time(nullptr);
            highScores[i] = {score, *localtime(&now)};
            break;
        }
    }

    outFile.write(reinterpret_cast<const char*>(&highScores), sizeof(highScores));
    outFile.close();
}

void HighScores::render()
{
    highScoresBackground.render(SCREEN_WIDTH / 2 - highScoresBackground.getWidth() / 2, 130);
    backTexture.render(SCREEN_WIDTH / 2 - backTexture.getWidth() / 2, highScoresBackground.getHeight() + 83);

    //render scores

    static SDL_Color textColor = { 66, 158, 245, 255 };

    int numX = SCREEN_WIDTH / 2 - highScoresBackground.getWidth() / 2 + 45,
        scoreX = SCREEN_WIDTH / 2 + highScoresBackground.getWidth() / 2 - 45,
        dateX = numX + 60,

        mPosY = 165;
    LTexture scoreText;
    for (int i = 0; i < scoresCount; i++) {
        scoreText.loadFromRenderedText(std::to_string(i + 1) + '.', glueGunFonts[FontStyle::Regular], textColor);
        scoreText.render(numX, mPosY);

        scoreText.loadFromRenderedText(std::to_string(highScores[i].first), glueGunFonts[FontStyle::Regular], textColor);
        scoreText.render(scoreX - scoreText.getWidth(), mPosY);

        //not exist
        if (highScores[i].first != 0) {
            scoreText.loadFromRenderedText(formatTime(highScores[i].second), glueGunFonts[FontStyle::Regular], textColor);
            scoreText.render(dateX, mPosY);
        }

        mPosY += 35;
    }
}

#endif //HIGHSCORES_H
