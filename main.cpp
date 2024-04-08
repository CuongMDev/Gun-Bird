#include "Main/mainFunctions.h"
#include "objects/Game/Game.h"
#include "objects/Menu/Menu.h"

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

        Menu menu;

        while (!quit)
        {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            //Handle events on list
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            menu.handleKey(currentKeyStates);
            while (SDL_PollEvent(e) != 0) {
                //User requests quit
                if (e->type == SDL_QUIT) {
                    quit = true;
                }

                //Handle input
                menu.handleEvent(e);
            }

            //Render
            menu.render();
            cursorMouse->render();

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}