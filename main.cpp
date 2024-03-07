#include "mainFunctions.h"
#include "objects.h"
#include <list>

//pipe will be created when current time = this
Uint32 createdTime = 2000;
void renderPipe(std::list<Pipe*>& pipeList);

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

            mainBird.move(ground);

            //Render
            background.render();
            ground.render();
            mainBird.render();
            renderPipe(pipeList);

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

void renderPipe(std::list<Pipe*>& pipeList)
{
    if (SDL_GetTicks() >= createdTime) {
        //random flip
        bool flip = getRandomNumber(0, 1);
        //pipe height
        int pipeHeight = getRandomNumber(50, 300);
        //create new pipe
        Pipe* pipe = new Pipe(getRandomNumber(50, SCREEN_HEIGHT / 2), flip);
        pipeList.push_back(pipe);

        //reset created time
        createdTime = SDL_GetTicks() + 2000 + getRandomNumber(1000, 3000);
    }

    //render pipe
    for (auto pipe = pipeList.begin(); pipe != pipeList.end();) {
        if (!(*pipe)->render()) {
            delete (*pipe);
            pipe = pipeList.erase(pipe);
        }
        else pipe++;
    }
}
