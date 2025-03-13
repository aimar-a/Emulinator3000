#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <dirent.h> // Necesario para leer el contenido de un directorio
#include "chip8_cpu.h"

int selectedDelay = 5;                                                                  // Valor inicial de SDL_Delay
char selectedRom[128] = "resources/chip8-roms/games/Space Invaders [David Winter].ch8"; // ROM seleccionada
int romListVisible = 0;                                                                 // Bandera para mostrar u ocultar la lista de ROMs
int scrollOffset = 0;                                                                   // Indica el desplazamiento de la lista
const int maxVisibleRoms = 3;                                                           // Máximo de ROMs visibles sin desplazamiento

// Función para cargar las ROMs del directorio
void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount)
{
    DIR *dir = opendir(dirPath); // Abre la carpeta que contiene las ROMs
    if (dir == NULL)
    {
        printf("No se pudo abrir el directorio %s\n", dirPath);
        return;
    }

    struct dirent *entry;
    *romCount = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        // Verifica si el archivo es una ROM (por ejemplo, archivos que terminan en ".ch8")
        if (strstr(entry->d_name, ".ch8") != NULL)
        {
            strcpy(romOptions[*romCount], entry->d_name);
            (*romCount)++;
        }
    }

    closedir(dir); // Cierra el directorio
}

void showSettingsWindow()
{
    SDL_Window *settingsWindow = SDL_CreateWindow("Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer *settingsRenderer = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);

    SDL_Event event;
    int running = 1;
    char delayText[10] = "";
    int delayInputActive = 0;

    // Cargar las ROMs desde un directorio
    char romOptions[100][128]; // Asumiendo que hay un máximo de 100 ROMs
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

        // Evento de entrada
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) // Permite cerrar con la "X" de la ventana
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) // Cerrar con ESC
                {
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_RETURN && delayInputActive)
                {
                    selectedDelay = atoi(delayText);
                    delayInputActive = 0;
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    if (romListVisible && romCount > 0)
                    {
                        romIndex = (romIndex - 1 + romCount) % romCount;
                        if (romIndex < scrollOffset)
                        {
                            scrollOffset--;
                        }
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (romListVisible && romCount > 0)
                    {
                        romIndex = (romIndex + 1) % romCount;
                        if (romIndex >= scrollOffset + maxVisibleRoms)
                        {
                            scrollOffset++;
                        }
                    }
                }
                else if (event.key.keysym.sym == SDLK_RETURN && romListVisible)
                {
                    strcpy(selectedRom, romOptions[romIndex]);
                    romListVisible = 0;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Verificar si se hizo clic en la ROM seleccionada
                if (x >= 50 && x <= 350 && y >= 150 && y <= 180)
                {
                    romListVisible = !romListVisible;
                }

                // Cerrar ventana si se hace clic en un botón imaginario de "Cerrar"
                if (x >= 360 && x <= 390 && y >= 10 && y <= 40)
                {
                    running = 0;
                }

                // Verificar si se hizo clic en una ROM de la lista desplegada
                if (romListVisible)
                {
                    for (int i = 0; i < romCount; ++i)
                    {
                        int romY = 190 + i * 40;
                        if (x >= 50 && x <= 350 && y >= romY && y <= romY + 40)
                        {
                            strcpy(selectedRom, romOptions[i]);
                            romListVisible = 0;
                            break;
                        }
                    }
                }
            }
        }

        // Dibujar la UI de ajustes
        SDL_SetRenderDrawColor(settingsRenderer, 255, 255, 255, 255);
        SDL_Rect inputBoxRect = {50, 50, 300, 30};
        SDL_RenderDrawRect(settingsRenderer, &inputBoxRect);

        SDL_Surface *inputTextSurface = TTF_RenderText_Solid(font, "Delay:", (SDL_Color){255, 255, 255});
        SDL_Texture *inputTextTexture = SDL_CreateTextureFromSurface(settingsRenderer, inputTextSurface);
        SDL_Rect inputTextRect = {50, 50, inputTextSurface->w, inputTextSurface->h};
        SDL_RenderCopy(settingsRenderer, inputTextTexture, NULL, &inputTextRect);
        SDL_FreeSurface(inputTextSurface);
        SDL_DestroyTexture(inputTextTexture);

        // Mostrar la ROM seleccionada
        SDL_Surface *selectedRomSurface = TTF_RenderText_Solid(font, selectedRom, (SDL_Color){255, 255, 255});
        SDL_Texture *selectedRomTexture = SDL_CreateTextureFromSurface(settingsRenderer, selectedRomSurface);
        SDL_Rect selectedRomRect = {50, 150, selectedRomSurface->w, selectedRomSurface->h};
        SDL_RenderCopy(settingsRenderer, selectedRomTexture, NULL, &selectedRomRect);
        SDL_FreeSurface(selectedRomSurface);
        SDL_DestroyTexture(selectedRomTexture);

        // Mostrar la lista de ROMs
        if (romListVisible)
        {
            for (int i = 0; i < maxVisibleRoms && (i + scrollOffset) < romCount; ++i)
            {
                int romY = 190 + i * 40;
                SDL_Surface *romSurface = TTF_RenderText_Solid(font, romOptions[i + scrollOffset], (SDL_Color){255, 255, 255});
                SDL_Texture *romTexture = SDL_CreateTextureFromSurface(settingsRenderer, romSurface);
                SDL_Rect romRect = {50, romY, romSurface->w, romSurface->h};
                SDL_RenderCopy(settingsRenderer, romTexture, NULL, &romRect);

                if ((i + scrollOffset) == romIndex)
                {
                    SDL_SetRenderDrawColor(settingsRenderer, 0, 255, 0, 255);
                    SDL_RenderDrawRect(settingsRenderer, &romRect);
                }

                SDL_FreeSurface(romSurface);
                SDL_DestroyTexture(romTexture);
            }
        }

        SDL_RenderPresent(settingsRenderer);
    }

    // Cerrar y liberar recursos de la ventana de ajustes
    SDL_DestroyRenderer(settingsRenderer);
    SDL_DestroyWindow(settingsWindow);
    TTF_CloseFont(font);
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

    SDL_Surface *settingsButtonSurface = SDL_LoadBMP("resources/Fotos/Settings.bmp");
    SDL_Texture *settingsButtonTexture = SDL_CreateTextureFromSurface(renderer, settingsButtonSurface);
    SDL_FreeSurface(settingsButtonSurface);

    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
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
                {
                    running = 0;
                    chip8cpuLaunch(selectedRom);
                }
                else if (x >= 220 && x <= 420 && y >= 250 && y <= 300)
                {
                    showSettingsWindow();
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
    SDL_Quit();
}