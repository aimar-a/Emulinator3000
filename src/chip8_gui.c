#include "chip8_cpu.h"
#include "chip8_structure.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include "chip8_opcodes.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void showInitialWindow(int *startGame)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    int running = 1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Emulinator3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    SDL_Surface *playButtonSurface = SDL_LoadBMP("C:/ProgIV/Emulinator3000/resources/Fotos/Playh.bmp");
    if (playButtonSurface == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "C:/ProgIV/Emulinator3000/resources/Fotos/Playh.bmp", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    SDL_FreeSurface(playButtonSurface);

    SDL_Surface *settingsButtonSurface = SDL_LoadBMP("C:/ProgIV/Emulinator3000/resources/Fotos/Settings.bmp");
    if (settingsButtonSurface == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "C:/ProgIV/Emulinator3000/resources/Fotos/Settings.bmp", SDL_GetError());
        SDL_DestroyTexture(playButtonTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *settingsButtonTexture = SDL_CreateTextureFromSurface(renderer, settingsButtonSurface);
    SDL_FreeSurface(settingsButtonSurface);

    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                *startGame = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 220 && x <= 420 && y >= 150 && y <= 200) // Play button area
                {
                    running = 0;
                    *startGame = 1;
                }
                else if (x >= 220 && x <= 420 && y >= 250 && y <= 300) // Settings button area
                {
                    // Handle settings button click
                    printf("Settings button clicked\n");
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black background
        SDL_RenderClear(renderer);

        SDL_Rect playButton = {220, 150, 200, 50};
        SDL_Rect settingsButton = {220, 250, 200, 50};

        SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton);
        SDL_RenderCopy(renderer, settingsButtonTexture, NULL, &settingsButton);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(settingsButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
