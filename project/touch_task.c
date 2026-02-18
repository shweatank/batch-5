#include <SDL2/SDL.h>
#include <stdio.h>

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    int width = 800;
    int height = 600;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Init Failed\n");
        return 1;
    }

    window = SDL_CreateWindow("Touch Window",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              0);

    if (!window)
    {
        printf("Window Creation Failed\n");
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    printf("SDL2 Window Started...\n");

    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                printf("Clicked at X:%d Y:%d\n", x, y);

                if (x > width/2 && y < height/2)
                {
                    printf("RIGHT TOP → LED FLOW\n");
                }
                else if (x > width/2 && y > height/2)
                {
                    printf("RIGHT BOTTOM → LED BLINK\n");
                }
                else if (x < width/2 && y < height/2)
                {
                    printf("LEFT TOP → UART MESSAGE\n");
                }
                else
                {
                    printf("LEFT BOTTOM → LED ON\n");
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
