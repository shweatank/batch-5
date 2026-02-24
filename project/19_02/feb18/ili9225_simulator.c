/*
 * ILI9225 LCD Simulator (176x220)
 * Runs on x86 PC using SDL2
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define LCD_WIDTH  176
#define LCD_HEIGHT 220
#define SCALE      3   // Makes window bigger for visibility

SDL_Window   *window;
SDL_Renderer *renderer;

/* Convert RGB565 to SDL RGB888 */
void rgb565_to_rgb888(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = ((color >> 11) & 0x1F) << 3;
    *g = ((color >> 5)  & 0x3F) << 2;
    *b = ( color        & 0x1F) << 3;
}

void lcd_init()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "ILI9225 LCD Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LCD_WIDTH * SCALE,
        LCD_HEIGHT * SCALE,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void lcd_drawPixel(int x, int y, uint16_t color)
{
    uint8_t r,g,b;
    rgb565_to_rgb888(color, &r, &g, &b);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    SDL_Rect rect = {
        x * SCALE,
        y * SCALE,
        SCALE,
        SCALE
    };

    SDL_RenderFillRect(renderer, &rect);
}

void lcd_fill(uint16_t color)
{
    for(int y=0; y<LCD_HEIGHT; y++)
        for(int x=0; x<LCD_WIDTH; x++)
            lcd_drawPixel(x,y,color);

    SDL_RenderPresent(renderer);
}

/* Simple A,B,C font */
const uint8_t font[3][8] = {
    {0x18,0x3C,0x24,0x66,0x7E,0x66,0x66,0x00}, // A
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
};

void lcd_drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 'A' || c > 'C') return;

    const uint8_t *bitmap = font[c - 'A'];

    for(int row=0; row<8; row++)
        for(int col=0; col<8; col++)
            if(bitmap[row] & (1 << (7-col)))
                lcd_drawPixel(x+col, y+row, color);

    SDL_RenderPresent(renderer);
}

void lcd_close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    lcd_init();

    lcd_fill(0xF800); SDL_Delay(500);  // Red
    lcd_fill(0x07E0); SDL_Delay(500);  // Green
    lcd_fill(0x001F); SDL_Delay(500);  // Blue
    lcd_fill(0xFFFF); SDL_Delay(500);  // White

    lcd_drawChar(10,10,'A',0xF800);
    SDL_Delay(1000);
    lcd_drawChar(30,10,'B',0xF800);
    SDL_Delay(1000);
    lcd_drawChar(50,10,'C',0xf800);
    SDL_Delay(1000);

    SDL_RenderPresent(renderer);

    printf("Close window to exit...\n");

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = 1;
    }

    lcd_close();
    return 0;
}

