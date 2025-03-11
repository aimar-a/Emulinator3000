#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Necesario para texto en la UI
#include <string.h>

int selectedDelay = 5;                 // Valor inicial de SDL_Delay
char selectedRom[128] = "default.ch8"; // ROM seleccionada

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

    char *romOptions[] = {"default.ch8", "pong.ch8", "tetris.ch8"};
    int romIndex = 0;

    while (running)
    {
        SDL_SetRenderDrawColor(settingsRenderer, 0, 0, 0, 255);
        SDL_RenderClear(settingsRenderer);

        // Evento de entrada
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN && delayInputActive)
                {
                    selectedDelay = atoi(delayText);
                    delayInputActive = 0;
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    romIndex = (romIndex + 1) % 3;
                    strcpy(selectedRom, romOptions[romIndex]);
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    romIndex = (romIndex - 1 + 3) % 3;
                    strcpy(selectedRom, romOptions[romIndex]);
                }
            }
            else if (event.type == SDL_TEXTINPUT && delayInputActive)
            {
                strcat(delayText, event.text.text);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 50 && x <= 350 && y >= 50 && y <= 80)
                {
                    delayInputActive = 1;
                    strcpy(delayText, "");
                }
            }
        }

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Delay:", (SDL_Color){255, 255, 255});
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(settingsRenderer, textSurface);
        SDL_Rect textRect = {50, 20, textSurface->w, textSurface->h};
        SDL_RenderCopy(settingsRenderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        SDL_Surface *romSurface = TTF_RenderText_Solid(font, selectedRom, (SDL_Color){255, 255, 255});
        SDL_Texture *romTexture = SDL_CreateTextureFromSurface(settingsRenderer, romSurface);
        SDL_Rect romRect = {50, 150, romSurface->w, romSurface->h};
        SDL_RenderCopy(settingsRenderer, romTexture, NULL, &romRect);
        SDL_FreeSurface(romSurface);
        SDL_DestroyTexture(romTexture);

        SDL_RenderPresent(settingsRenderer);
    }

    SDL_DestroyRenderer(settingsRenderer);
    SDL_DestroyWindow(settingsWindow);
    TTF_Quit();
}

void showInitialWindow(int *startGame)
{
    SDL_Window *window = SDL_CreateWindow("Emulinator3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    int running = 1;

    SDL_Surface *playButtonSurface = SDL_LoadBMP("C:/ProgIV/Emulinator3000/resources/Fotos/Playh.bmp");
    SDL_Texture *playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    SDL_FreeSurface(playButtonSurface);

    SDL_Surface *settingsButtonSurface = SDL_LoadBMP("C:/ProgIV/Emulinator3000/resources/Fotos/Settings.bmp");
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
                if (x >= 220 && x <= 420 && y >= 150 && y <= 200)
                {
                    running = 0;
                    *startGame = 1;
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
}
