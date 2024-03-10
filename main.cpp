#include "mainFunctions.h"
#include "Game.h"

int main(int argc, char *args[])
{
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        //Main loop flag
        bool quit = false;
        //Event handler
        SDL_Event *e = new SDL_Event();

        Game game;

        while (!quit)
        {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            //Handle events on list
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            game.handleKey(currentKeyStates);
            while (SDL_PollEvent(e) != 0) {
                //User requests quit
                if (e->type == SDL_QUIT) {
                    quit = true;
                }

                //Handle input for the Main Bird
                game.handleEvent(e);
            }

            //Render
            game.render();

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}