#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <dirent.h>
#include "chip8_cpu.h"

int selectedDelay = 5;
char selectedRom[128] = "resources/chip8-roms/games/Space Invaders [David Winter].ch8";
int romListVisible = 0;
int scrollOffset = 0;
const int maxVisibleRoms = 3;

void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount)
{
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        printf("No se pudo abrir el directorio %s\n", dirPath);
        return;
    }

    struct dirent *entry;
    *romCount = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".ch8") != NULL)
        {
            strcpy(romOptions[*romCount], entry->d_name);
            (*romCount)++;
        }
    }

    closedir(dir);
}

int showSettingsWindow(char selectedRom[128])
{
    SDL_Window *settingsWindow = SDL_CreateWindow("Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 350, SDL_WINDOW_SHOWN);
    SDL_Renderer *settingsRenderer = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);

    SDL_Event event;
    int running = 1;
    int playGame = 0;

    char romOptions[100][128];
    int romCount = 0;
    loadRomsFromDirectory("resources/chip8-roms/games", romOptions, &romCount);

    int romIndex = 0;
    if (romCount > 0)
    {
        strcpy(selectedRom, romOptions[0]);
    }

    SDL_StartTextInput();

    while (running)
    {
        SDL_SetRenderDrawColor(settingsRenderer, 0, 0, 0, 255);
        SDL_RenderClear(settingsRenderer);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_UP && romListVisible)
                {
                    // Navegar hacia arriba en la lista
                    romIndex = (romIndex - 1 + romCount) % romCount;
                    if (romIndex < scrollOffset)
                    {
                        scrollOffset--;
                    }
                    strcpy(selectedRom, romOptions[romIndex]); // Actualizar la ROM seleccionada
                }
                else if (event.key.keysym.sym == SDLK_DOWN && romListVisible)
                {
                    // Navegar hacia abajo en la lista
                    romIndex = (romIndex + 1) % romCount;
                    if (romIndex >= scrollOffset + maxVisibleRoms)
                    {
                        scrollOffset++;
                    }
                    strcpy(selectedRom, romOptions[romIndex]); // Actualizar la ROM seleccionada
                }
                else if (event.key.keysym.sym == SDLK_RETURN && romListVisible)
                {
                    // Seleccionar la ROM actual con Enter
                    strcpy(selectedRom, romOptions[romIndex]);
                    romListVisible = 0;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Botón "Jugar" en settings
                if (x >= 50 && x <= 350 && y >= 250 && y <= 300)
                {
                    playGame = 1;
                    running = 0;
                }

                // Mostrar/ocultar la lista de ROMs
                if (x >= 50 && x <= 350 && y >= 150 && y <= 180)
                {
                    romListVisible = !romListVisible;
                }

                // Seleccionar una ROM de la lista con el ratón
                if (romListVisible)
                {
                    for (int i = 0; i < romCount; ++i)
                    {
                        int romY = 190 + i * 40;
                        if (x >= 50 && x <= 350 && y >= romY && y <= romY + 40)
                        {
                            strcpy(selectedRom, romOptions[i]);
                            romListVisible = 0;
                            romIndex = i; // Actualizar el índice de la ROM seleccionada
                            break;
                        }
                    }
                }
            }
        }

        // Dibujar la ROM seleccionada
        SDL_Surface *selectedRomSurface = TTF_RenderText_Solid(font, selectedRom, (SDL_Color){255, 255, 255});
        SDL_Texture *selectedRomTexture = SDL_CreateTextureFromSurface(settingsRenderer, selectedRomSurface);
        SDL_Rect selectedRomRect = {50, 150, selectedRomSurface->w, selectedRomSurface->h};
        SDL_RenderCopy(settingsRenderer, selectedRomTexture, NULL, &selectedRomRect);
        SDL_FreeSurface(selectedRomSurface);
        SDL_DestroyTexture(selectedRomTexture);

        // Dibujar la lista de ROMs si está visible
        if (romListVisible)
        {
            for (int i = 0; i < maxVisibleRoms && (i + scrollOffset) < romCount; ++i)
            {
                int romY = 190 + i * 40;
                SDL_Surface *romSurface = TTF_RenderText_Solid(font, romOptions[i + scrollOffset], (SDL_Color){255, 255, 255});
                SDL_Texture *romTexture = SDL_CreateTextureFromSurface(settingsRenderer, romSurface);
                SDL_Rect romRect = {50, romY, romSurface->w, romSurface->h};
                SDL_RenderCopy(settingsRenderer, romTexture, NULL, &romRect);

                // Resaltar la ROM seleccionada
                if ((i + scrollOffset) == romIndex)
                {
                    SDL_SetRenderDrawColor(settingsRenderer, 0, 255, 0, 255);
                    SDL_RenderDrawRect(settingsRenderer, &romRect);
                }

                SDL_FreeSurface(romSurface);
                SDL_DestroyTexture(romTexture);
            }
        }

        // Dibujar el botón "Jugar"
        SDL_Surface *playSurface = TTF_RenderText_Solid(font, "Jugar", (SDL_Color){255, 255, 255});
        SDL_Texture *playTexture = SDL_CreateTextureFromSurface(settingsRenderer, playSurface);
        SDL_Rect playRect = {50, 250, playSurface->w, playSurface->h};
        SDL_RenderCopy(settingsRenderer, playTexture, NULL, &playRect);
        SDL_FreeSurface(playSurface);
        SDL_DestroyTexture(playTexture);

        SDL_RenderPresent(settingsRenderer);
    }

    // Limpieza y lanzar juego si es necesario
    SDL_DestroyRenderer(settingsRenderer);
    SDL_DestroyWindow(settingsWindow);
    TTF_CloseFont(font);

    if (playGame)
    {
        char fullRomPath[256]; // Buffer para la ruta completa
        sprintf(fullRomPath, "resources/chip8-roms/games/%s", selectedRom);

        chip8cpuLaunch(fullRomPath);
    }

    return 1;
}

void showInitialWindow()
{
    SDL_Window *window = SDL_CreateWindow("Emulinator3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    int running = 1;

    SDL_Surface *playButtonSurface = SDL_LoadBMP("resources/Fotos/Playh.bmp");
    SDL_Texture *playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    SDL_FreeSurface(playButtonSurface);

    char selectedRom[128] = "";

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 220 && x <= 420 && y >= 150 && y <= 200)
                { // Botón "Jugar"
                    running = 0;
                    SDL_DestroyTexture(playButtonTexture);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);

                    if (showSettingsWindow(selectedRom))
                    {
                        showInitialWindow();
                    }
                    return;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect playButton = {220, 150, 200, 50};
        SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}