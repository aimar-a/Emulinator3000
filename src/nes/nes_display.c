#include "nes_display.h"



static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

void nes_display_init()
{
  leer_configuracion("resources/config/config", "SCREEN_WIDTH_NES");  // Ruta relativa al archivo de configuración

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        nes_log("ERROR: SDL_Init failed: %s\n", SDL_GetError());
        return;
    }
    nes_log("INFO: SDL_Init successful\n");

    window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, 0);
    if (!window)
    {
        nes_log("ERROR: SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    nes_log("INFO: SDL_CreateWindow successful\n");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        nes_log("ERROR: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    nes_log("INFO: SDL_CreateRenderer successful\n");

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!texture)
    {
        nes_log("ERROR: SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    nes_log("INFO: SDL_CreateTexture successful\n");
}

void nes_display_draw(uint8_t *pantalla)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int j = 0; j < SCREEN_HEIGHT; j++)
    {
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            uint8_t color = pantalla[i + j * SCREEN_WIDTH];
            uint8_t r = color & 0x3;
            uint8_t g = (color >> 2) & 0x3;
            uint8_t b = (color >> 4) & 0x3;
            SDL_SetRenderDrawColor(renderer, r * 85, g * 85, b * 85, 255);
            SDL_Rect pixel = {i * SCREEN_SCALE, j * SCREEN_SCALE, SCREEN_SCALE, SCREEN_SCALE};
            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    SDL_RenderPresent(renderer);
}

void nes_display_destroy()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    nes_log("INFO: SDL resources destroyed and SDL quit\n");
}
