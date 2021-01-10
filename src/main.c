#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    if (0 > SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("%s: %s.", __func__, SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
