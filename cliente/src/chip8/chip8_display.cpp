#include "chip8_display.hpp"
#include <stdexcept>

Chip8Display::Chip8Display()
    : window(nullptr, SDL_DestroyWindow),
      renderer(nullptr, SDL_DestroyRenderer)
{
}

Chip8Display::~Chip8Display()
{
  destroy();
}

bool Chip8Display::initialize(bool superChipMode)
{
  if (superChipMode)
  {
    SCREEN_WIDTH_CHIP8 = SCREEN_WIDTH_SUPERCHIP;
    SCREEN_HEIGHT_CHIP8 = SCREEN_HEIGHT_SUPERCHIP;
    SCREEN_SCALE_CHIP8 = SCREEN_SCALE_SUPERCHIP;
    logInfo("SuperChip8 mode activated. Resolution: " +
            std::to_string(SCREEN_WIDTH_CHIP8) + "x" +
            std::to_string(SCREEN_HEIGHT_CHIP8));
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    logError("Failed to initialize SDL: " + std::string(SDL_GetError()));
    return false;
  }

  window.reset(SDL_CreateWindow(
      "CHIP-8 Emulator",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH_CHIP8 * SCREEN_SCALE_CHIP8,
      SCREEN_HEIGHT_CHIP8 * SCREEN_SCALE_CHIP8,
      SDL_WINDOW_SHOWN));

  if (!window)
  {
    logError("Failed to create window: " + std::string(SDL_GetError()));
    SDL_Quit();
    return false;
  }
  logInfo("Window created successfully");

  renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (!renderer)
  {
    logError("Failed to create renderer: " + std::string(SDL_GetError()));
    window.reset();
    SDL_Quit();
    return false;
  }
  logInfo("Renderer created successfully");

  return true;
}

void Chip8Display::destroy()
{
  if (renderer)
  {
    renderer.reset();
    logInfo("Renderer destroyed");
  }

  if (window)
  {
    window.reset();
    logInfo("Window destroyed");
  }

  SDL_Quit();
  logInfo("SDL quit");
}

void Chip8Display::render(const uint8_t *screenBuffer)
{
  // Clear screen
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(renderer.get());

  // Draw pixels
  SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
  for (int y = 0; y < SCREEN_HEIGHT_CHIP8; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH_CHIP8; ++x)
    {
      if (screenBuffer[x + y * SCREEN_WIDTH_CHIP8])
      {
        SDL_Rect pixel = {
            x * SCREEN_SCALE_CHIP8,
            y * SCREEN_SCALE_CHIP8,
            SCREEN_SCALE_CHIP8,
            SCREEN_SCALE_CHIP8};
        SDL_RenderFillRect(renderer.get(), &pixel);
      }
    }
  }

  // Update screen
  SDL_RenderPresent(renderer.get());
}

void Chip8Display::logError(const std::string &message) const
{
  std::cerr << "ERROR: " << message << std::endl;
}

void Chip8Display::logInfo(const std::string &message) const
{
  std::cout << "INFO: " << message << std::endl;
}