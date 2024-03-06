#include "mainData.h"
#include "objects.h"

void close()
{
    //Destroy window	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

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
        while (!quit)
        {
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            //Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                //User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                //Handle input for the Main Bird
                mainBird.handleEvent(e);
            }

            mainBird.move(ground);

            //Render
            background.render();
            ground.render();
            mainBird.render();

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
