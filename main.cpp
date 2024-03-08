#include "mainFunctions.h"

int main(int argc, char* args[])
{
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        //Main loop flag
        bool quit = false;
        //Event handler
        SDL_Event e;

        Background background;
        Ground ground(groundPosX, groundPosY);
        MainBird mainBird(mainBirdPosX, mainBirdPosY);

        std::list<Pipe*> pipeList;
        while (!quit)
        {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            //Handle events on list
            while (SDL_PollEvent(&e) != 0) {
                //User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                //Handle input for the Main Bird
                mainBird.handleEvent(e);
            }

            //Render
            background.render();
            ground.render();
            renderPipe(pipeList);
            mainBird.render(ground, pipeList);

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}