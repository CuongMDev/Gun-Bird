#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL_ttf.h>
#include "../../Main/mainData.h"
#include <map>

const int fontSize = 28;

enum class FontStyle {
    Bold,
    Light,
    Medium,
    Regular,
    SemiBold
};

std::map<FontStyle, TTF_Font*> firaFonts;

void loadFontFromFile(std::map<FontStyle, TTF_Font*> &fontContainer, std::string path, FontStyle fontStyle)
{
    fontContainer[fontStyle] = TTF_OpenFont(path.c_str(), fontSize );
    if( fontContainer[fontStyle] == NULL ) {
        printf( "Failed to load font! SDL_ttf Error: %s %s\n", path.c_str(), TTF_GetError() );
    }
}

void loadFiraFont()
{
    loadFontFromFile(firaFonts, firaCodeFontPath + "FiraCode-Bold.ttf", FontStyle::Bold);
    loadFontFromFile(firaFonts, firaCodeFontPath + "FiraCode-Light.ttf", FontStyle::Light);
    loadFontFromFile(firaFonts, firaCodeFontPath + "FiraCode-Medium.ttf", FontStyle::Medium);
    loadFontFromFile(firaFonts, firaCodeFontPath + "FiraCode-Regular.ttf", FontStyle::Regular);
    loadFontFromFile(firaFonts, firaCodeFontPath + "FiraCode-SemiBold.ttf", FontStyle::SemiBold);
}

void loadFont()
{
    loadFiraFont();
}

#endif //FONT_H
