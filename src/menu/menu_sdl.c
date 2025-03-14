#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <dirent.h>
#include "chip8_cpu.h"
#include "menu_sdl.h"

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

bool isRomSelected(char romOptions[][128], char selectedRom[128], int romCount)
{
    for (int i = 0; i < romCount; i++)
    {
        if (strcmp(selectedRom, romOptions[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void showWarningWindow()
{
    SDL_Window *warningWindow = SDL_CreateWindow("Error de ROMS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer *warningRenderer = SDL_CreateRenderer(warningWindow, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("resources/fonts/Arial.ttf", 18); // Fuente más pequeña, más parecida al estilo de Windows

    SDL_Event event;
    int running = 1;

    while (running)
    {

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 0};
        SDL_SetRenderDrawColor(warningRenderer, 240, 240, 240, 255); // Fondo gris claro, similar al estilo de Windows
        SDL_RenderClear(warningRenderer);

        // Dibujar el borde de la ventana (como la típica ventana de error de Windows)
        SDL_SetRenderDrawColor(warningRenderer, 0, 0, 0, 255); // Borde negro
        SDL_Rect borderRect = {0, 0, 400, 200};
        SDL_RenderDrawRect(warningRenderer, &borderRect);

        // Dibujar el mensaje de advertencia
        SDL_Surface *messageSurface = TTF_RenderText_Solid(font, " Debes seleccionar una ROM!", black);
        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(warningRenderer, messageSurface);
        SDL_Rect messageRect = {80, 40, messageSurface->w, messageSurface->h}; // Mensaje centrado
        SDL_RenderCopy(warningRenderer, messageTexture, NULL, &messageRect);
        SDL_FreeSurface(messageSurface);
        SDL_DestroyTexture(messageTexture);

        // Dibujar el rectángulo para el botón "Aceptar"
        SDL_SetRenderDrawColor(warningRenderer, 0, 120, 215, 255);   // Borde del botón en negro
        SDL_Rect buttonRectBackground = {140, 120, 120, 40};         // Rectángulo donde va el botón
        SDL_RenderFillRect(warningRenderer, &buttonRectBackground);  // Fondo del botón
        SDL_SetRenderDrawColor(warningRenderer, 255, 255, 255, 255); // Color del borde del botón (blanco)
        SDL_RenderDrawRect(warningRenderer, &buttonRectBackground);  // Borde del botón

        // Dibujar el texto del botón "Aceptar"

        SDL_Surface *buttonSurface = TTF_RenderText_Solid(font, "Aceptar", white);
        SDL_Texture *buttonTexture = SDL_CreateTextureFromSurface(warningRenderer, buttonSurface);
        SDL_Rect buttonTextRect = {170, 130, buttonSurface->w, buttonSurface->h}; // Texto centrado dentro del botón
        SDL_RenderCopy(warningRenderer, buttonTexture, NULL, &buttonTextRect);
        SDL_FreeSurface(buttonSurface);
        SDL_DestroyTexture(buttonTexture);

        SDL_RenderPresent(warningRenderer);

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

                if (x >= 140 && x <= 140 + buttonRectBackground.w && y >= 120 && y <= 120 + buttonRectBackground.h)
                {
                    running = 0; // Cerrar la ventana si se hace clic en "Aceptar"
                }
            }
        }
    }

    SDL_DestroyRenderer(warningRenderer);
    SDL_DestroyWindow(warningWindow);
    TTF_CloseFont(font);
}

int showSettingsWindow(char selectedRom[128])
{
    SDL_Window *settingsWindow = SDL_CreateWindow("Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *settingsRenderer = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("resources/fonts/Arial.ttf", 24); // Tamaño de fuente

    SDL_Event event;
    int running = 1;
    int playGame = 0;

    char romOptions[107][128];
    int romCount = 0;
    loadRomsFromDirectory("resources/chip8-roms/games", romOptions, &romCount);

    int romsPerPage = 15;                                        // ROMs por página (5 filas x 3 columnas)
    int currentPage = 0;                                         // Página actual
    int totalPages = (romCount + romsPerPage - 1) / romsPerPage; // Total de páginas

    SDL_StartTextInput();

    while (running)
    {
        int winW, winH;
        SDL_GetWindowSize(settingsWindow, &winW, &winH); // Obtener tamaño actual de la ventana

        SDL_SetRenderDrawColor(settingsRenderer, 0, 0, 0, 255);
        SDL_RenderClear(settingsRenderer);

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

                // Botón "Jugar"
                int playButtonX = winW / 2 - 100;
                int playButtonY = winH - 120;
                if (x >= playButtonX && x <= playButtonX + 200 && y >= playButtonY && y <= playButtonY + 60 && isRomSelected(romOptions, selectedRom, romCount))
                {
                    playGame = 1;
                    running = 0;
                }
                else if ((isRomSelected(romOptions, selectedRom, romCount) == false) && x >= playButtonX && x <= playButtonX + 200 && y >= playButtonY && y <= playButtonY + 60)
                {
                    showWarningWindow();
                }

                // Botón "Anterior"
                int prevButtonX = winW / 4 - 50;
                if (x >= prevButtonX && x <= prevButtonX + 200 && y >= playButtonY && y <= playButtonY + 60)
                {
                    if (currentPage > 0)
                    {
                        currentPage--;
                    }
                }

                // Botón "Siguiente"
                int nextButtonX = (winW * 3) / 4 - 50;
                if (x >= nextButtonX && x <= nextButtonX + 200 && y >= playButtonY && y <= playButtonY + 60)
                {
                    if (currentPage < totalPages - 1)
                    {
                        currentPage++;
                    }
                }

                // Seleccionar una ROM de la lista
                int gridStartX = winW / 2 - (3 * 350) / 2; // Centrar la cuadrícula en la ventana
                int gridStartY = 100;

                for (int i = 0; i < romsPerPage; ++i)
                {
                    int romIndex = currentPage * romsPerPage + i;
                    if (romIndex >= romCount)
                        break;

                    int col = i % 3;
                    int row = i / 3;

                    int romX = 100 + gridStartX + col * 350;
                    int romY = gridStartY + row * 50;

                    if (x >= romX && x <= romX + 300 && y >= romY && y <= romY + 40)
                    {
                        strcpy(selectedRom, romOptions[romIndex]);
                    }
                }
            }
        }

        // Dibujar la lista de ROMs
        int gridStartX = winW / 2 - (3 * 350) / 2; // Centrar la cuadrícula
        int gridStartY = 100;

        for (int i = 0; i < romsPerPage; ++i)
        {
            int romIndex = currentPage * romsPerPage + i;
            if (romIndex >= romCount)
                break;

            int col = i % 3;
            int row = i / 3;

            int romX = 100 + gridStartX + col * 350;
            int romY = gridStartY + row * 50;

            char displayName[128];
            strcpy(displayName, romOptions[romIndex]);

            char *dot = strrchr(displayName, '.');
            if (dot != NULL)
                *dot = '\0';

            char *openBracket = strchr(displayName, '[');
            if (openBracket != NULL)
                *openBracket = '\0';

            char *openParen = strchr(displayName, '(');
            if (openParen != NULL)
                *openParen = '\0';

                        SDL_Color white = {255, 255, 255, 255};
            SDL_Surface *romSurface = TTF_RenderText_Solid(font, displayName, white);
            SDL_Texture *romTexture = SDL_CreateTextureFromSurface(settingsRenderer, romSurface);
            SDL_Rect romRect = {romX, romY, romSurface->w, romSurface->h};
            SDL_RenderCopy(settingsRenderer, romTexture, NULL, &romRect);

            if (strcmp(selectedRom, romOptions[romIndex]) == 0)
            {
                SDL_SetRenderDrawColor(settingsRenderer, 0, 255, 0, 255);
                SDL_RenderDrawRect(settingsRenderer, &romRect);
            }

            SDL_FreeSurface(romSurface);
            SDL_DestroyTexture(romTexture);
        }

        // Dibujar el botón "Jugar"
        int playButtonX = 30 + (winW / 2 - 100);
        int playButtonY = winH - 120;
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *playSurface = TTF_RenderText_Solid(font, "Jugar", white);
        SDL_Texture *playTexture = SDL_CreateTextureFromSurface(settingsRenderer, playSurface);
        SDL_Rect playRect = {playButtonX, playButtonY, playSurface->w, playSurface->h};
        SDL_RenderCopy(settingsRenderer, playTexture, NULL, &playRect);
        SDL_FreeSurface(playSurface);
        SDL_DestroyTexture(playTexture);

        // Dibujar el botón "Anterior"
        int prevButtonX = winW / 4 - 50;
        if (currentPage > 0)
        {

            SDL_Surface *prevSurface = TTF_RenderText_Solid(font, "Anterior", white);
            SDL_Texture *prevTexture = SDL_CreateTextureFromSurface(settingsRenderer, prevSurface);
            SDL_Rect prevRect = {prevButtonX, playButtonY, prevSurface->w, prevSurface->h};
            SDL_RenderCopy(settingsRenderer, prevTexture, NULL, &prevRect);
            SDL_FreeSurface(prevSurface);
            SDL_DestroyTexture(prevTexture);
        }

        // Dibujar el botón "Siguiente"
        int nextButtonX = (winW * 3) / 4 - 50;
        if (currentPage < totalPages - 1)
        {

            SDL_Surface *nextSurface = TTF_RenderText_Solid(font, "Siguiente", white);
            SDL_Texture *nextTexture = SDL_CreateTextureFromSurface(settingsRenderer, nextSurface);
            SDL_Rect nextRect = {nextButtonX, playButtonY, nextSurface->w, nextSurface->h};
            SDL_RenderCopy(settingsRenderer, nextTexture, NULL, &nextRect);
            SDL_FreeSurface(nextSurface);
            SDL_DestroyTexture(nextTexture);
        }

        SDL_RenderPresent(settingsRenderer);
    }

    SDL_DestroyRenderer(settingsRenderer);
    SDL_DestroyWindow(settingsWindow);
    TTF_CloseFont(font);

    if (playGame)
    {
        char fullRomPath[256];
        sprintf(fullRomPath, "resources/chip8-roms/games/%s", selectedRom);
        chip8cpuLaunch(fullRomPath);
    }

    return 1;
}

void showInitialWindow()
{
    SDL_Window *window = SDL_CreateWindow("Emulinator3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    int running = 1;

    SDL_Surface *playButtonSurface = SDL_LoadBMP("resources/Fotos/Playh.bmp");
    SDL_Texture *playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    SDL_FreeSurface(playButtonSurface);

    char selectedRom[128] = "";

    // Ajustar el tamaño y la posición del botón "Play"
    int buttonWidth = 400;                  // Ancho del botón
    int buttonHeight = 100;                 // Alto del botón
    int buttonX = (1280 - buttonWidth) / 2; // Centrar horizontalmente
    int buttonY = (720 - buttonHeight) / 2; // Centrar verticalmente

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
                if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight)
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

        SDL_Rect playButton = {buttonX, buttonY, buttonWidth, buttonHeight};
        SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}