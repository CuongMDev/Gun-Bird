#ifndef SETTING_H
#define SETTING_H

#include <fstream>
#include <filesystem>

class Setting
{
private:
    enum IMAGE
    {
        BACKGROUND,
        BAR,
        MUSIC,
        SOUND,
        TEXT,

        IMAGE_COUNT
    };

    enum VOLUME_TYPE
    {
        MUSIC_TYPE,
        CHUNK_TYPE,

        VOLUME_TYPE_COUNT
    };

    const int volumeBarPosX[VOLUME_TYPE_COUNT] = {495, 495};
    const int volumeBarPosY[VOLUME_TYPE_COUNT] = {130, 200};

    const int volumeBarWidth = 259;
    const int volumeBarHeight = 48;

    //VOLUME_TYPE_COUNT: none, i:collided with bar i
    VOLUME_TYPE mouseCollided;
    int currentVolumeWidth[VOLUME_TYPE_COUNT];

    static int currentChannel;
    static int currentVolumeCount[VOLUME_TYPE_COUNT];

    LTexture sTexture[IMAGE_COUNT];

    void setVolume(Setting::VOLUME_TYPE volumeType, int volumeWidth);
    static void updateChannelVolume();

    void init();
    void loadIMG();
    void loadSetting();
public:
    Setting();

    static void allocateChannels(int channelCount);

    void saveSetting();
    void handleEvent(SDL_Event *e);
    void render();
};

int Setting::currentChannel = 0;
int Setting::currentVolumeCount[VOLUME_TYPE_COUNT] = {};

Setting::Setting()
{
    init();
    loadIMG();
    loadSetting();
}

void Setting::init()
{
    mouseCollided = VOLUME_TYPE_COUNT;
    for (auto & volumeWidth : currentVolumeWidth) {
        volumeWidth = volumeBarWidth;
    }
    for (auto & volumeCount : currentVolumeCount) {
        volumeCount = MIX_MAX_VOLUME;
    }
}

void Setting::loadIMG()
{
    sTexture[BACKGROUND].loadFromFile(settingImagePath + "background.png");
    sTexture[BAR].loadFromFile(settingImagePath + "bar.png");
    sTexture[MUSIC].loadFromFile(settingImagePath + "music.png");
    sTexture[SOUND].loadFromFile(settingImagePath + "sound.png");
    sTexture[TEXT].loadFromFile(settingImagePath + "text.png");
}

void Setting::render()
{
    sTexture[BACKGROUND].render(SCREEN_WIDTH / 2 - sTexture[BACKGROUND].getWidth() / 2, 100);
    sTexture[MUSIC].render(SCREEN_WIDTH / 2 - sTexture[BACKGROUND].getWidth() / 2 + 70, 140);


    sTexture[SOUND].render(SCREEN_WIDTH / 2 - sTexture[BACKGROUND].getWidth() / 2 + 75, 210);

    sTexture[TEXT].render(SCREEN_WIDTH / 2 - sTexture[TEXT].getWidth() / 2 - 5, sTexture[BACKGROUND].getHeight() + 60);

    SDL_Rect clip;
    clip.x = 0, clip.y = 0;
    clip.w = currentVolumeWidth[MUSIC_TYPE], clip.h = volumeBarHeight;
    sTexture[BAR].render(volumeBarPosX[MUSIC_TYPE], volumeBarPosY[MUSIC_TYPE], &clip);

    clip.w = currentVolumeWidth[CHUNK_TYPE];
    sTexture[BAR].render(volumeBarPosX[CHUNK_TYPE], volumeBarPosY[CHUNK_TYPE], &clip);
}

void Setting::loadSetting()
{
    std::ifstream inFile(settingConfigPath);
    if (!inFile.is_open()) {
        return;
    }

    int musicVolumeWidth, chunkVolumeWidth;
    inFile >> musicVolumeWidth >> chunkVolumeWidth;

    setVolume(MUSIC_TYPE, musicVolumeWidth);
    setVolume(CHUNK_TYPE, chunkVolumeWidth);

    inFile.close();
}

void Setting::saveSetting()
{
    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directory(dataPath);
    }
    std::ofstream outFile(settingConfigPath);
    if (!outFile.is_open()) {
        return;
    }

    outFile << currentVolumeWidth[0] << ' ' << currentVolumeWidth[1];

    outFile.close();
}

void Setting::handleEvent(SDL_Event *e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            for (int volumeType = 0; volumeType < VOLUME_TYPE_COUNT; volumeType++) {
                if (cursorMouse->checkMouseCollision(volumeBarPosX[volumeType], volumeBarPosY[volumeType], volumeBarWidth, volumeBarHeight)) {
                    mouseCollided = static_cast<VOLUME_TYPE>(volumeType);
                    break;
                }
            }
        }
    }
    if (e->type == SDL_MOUSEBUTTONUP) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            //reset
            mouseCollided = VOLUME_TYPE_COUNT;
        }
    }

    if (mouseCollided != VOLUME_TYPE_COUNT) {
        if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            setVolume(mouseCollided, mouseX - volumeBarPosX[mouseCollided]);
        }
    }
}

void Setting::setVolume(Setting::VOLUME_TYPE volumeType, int volumeWidth) {
    if (volumeWidth < 0) volumeWidth = 0;
    if (volumeWidth > volumeBarWidth) volumeWidth = volumeBarWidth;
    if (volumeWidth == currentVolumeWidth[volumeType]) {
        return;
    }

    currentVolumeWidth[volumeType] = volumeWidth;
    currentVolumeCount[volumeType] = MIX_MAX_VOLUME * volumeWidth / volumeBarWidth;
    if (volumeType == MUSIC_TYPE) {
        Mix_VolumeMusic(currentVolumeCount[volumeType]);
    }
    else {
        updateChannelVolume();
    }
}

void Setting::allocateChannels(int channelCount) {
    currentChannel = channelCount;
    Mix_AllocateChannels(channelCount);
    updateChannelVolume();
}

void Setting::updateChannelVolume()
{
    for (int channel = 0; channel < currentChannel; channel++) {
        Mix_Volume(channel, currentVolumeCount[CHUNK_TYPE]);
    }
}

#endif //SETTING_H
