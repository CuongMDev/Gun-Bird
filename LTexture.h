#include <SDL2/SDL.h>

class LTexture
{
private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
public:
    LTexture(/* args */);
    ~LTexture();
};

LTexture::LTexture(/* args */)
{
}

LTexture::~LTexture()
{
}
