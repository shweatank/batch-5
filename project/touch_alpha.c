#include <SDL2/SDL.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 480

int fd;
int x = 0, y = 0;
int drawing = 0;

void save_image(SDL_Renderer *renderer)
{
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,
                                              0x00FF0000,
                                              0x0000FF00,
                                              0x000000FF,
                                              0xFF000000);

    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888,
                          sshot->pixels, sshot->pitch);

    SDL_SaveBMP(sshot, "draw.bmp");
    SDL_FreeSurface(sshot);

    printf("Image saved as draw.bmp\n");
}

void recognize_char()
{
    system("tesseract draw.bmp out -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZ --psm 10");

    FILE *fp = fopen("out.txt", "r");
    if (!fp) return;

    char buf[32];
    fgets(buf, sizeof(buf), fp);
    fclose(fp);

    printf("Recognized Alphabet: %s\n", buf);
}

int main()
{
    struct input_event ev;

    fd = open("/dev/input/event2", O_RDONLY); // change if needed
    if (fd < 0) {
        perror("Cannot open input device");
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Touch Draw",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    printf("Draw an alphabet using your finger...\n");

    while (1) {
        read(fd, &ev, sizeof(ev));

        if (ev.type == EV_ABS) {
            if (ev.code == ABS_X)
                x = ev.value;
            if (ev.code == ABS_Y)
                y = ev.value;

            if (drawing) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);
                SDL_RenderPresent(renderer);
            }
        }

        if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
            if (ev.value == 1) {
                drawing = 1;
                printf("Finger DOWN\n");
            } else {
                drawing = 0;
                printf("Finger UP -> Recognizing...\n");

                save_image(renderer);
                recognize_char();

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
            }
        }
    }

    close(fd);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
