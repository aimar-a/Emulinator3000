#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
  uint8_t spriteA[5] = {0xF0, 0x90, 0xF0, 0x90, 0x90};
  uint8_t spriteI[5] = {0xF0, 0x20, 0x20, 0x20, 0xF0};
  uint8_t spriteM[5] = {0x90, 0xF0, 0x90, 0x90, 0x90};
  uint8_t spriteO[5] = {0xF0, 0x90, 0x90, 0x90, 0xF0};
  uint8_t spriteN[5] = {0xF0, 0x90, 0x90, 0x90, 0x90};
  uint8_t spriteZ[5] = {0xF0, 0x10, 0x20, 0x40, 0xF0};
  uint8_t spriteS[5] = {0xF0, 0x80, 0xF0, 0x10, 0xF0};
  uint8_t spriteD[5] = {0xE0, 0x90, 0x90, 0x90, 0xE0};
  uint8_t spriteE[5] = {0xF0, 0x80, 0xF0, 0x80, 0xF0};
  uint8_t spriteV[5] = {0x90, 0x90, 0x90, 0x90, 0x60};

  Screen pantalla;
  iniciarPantalla(&pantalla);

  // Dibujar AIMONZAS
  drawSprite(10, 10, spriteA, 5);
  drawSprite(16, 10, spriteI, 5);
  drawSprite(28, 10, spriteO, 5);
  drawSprite(34, 10, spriteN, 5);
  drawSprite(22, 10, spriteM, 5);
  drawSprite(40, 10, spriteZ, 5);
  drawSprite(46, 10, spriteA, 5);
  drawSprite(52, 10, spriteS, 5);

  printPantalla(&pantalla);

  for (int i = 0; i < 50; i++)
  {
    capturarTeclado();
    uint8_t *pTeclado = getTeclado();
    printf("Teclas presionadas: ");
    for (int i = 0; i < 16; i++)
    {
      if (pTeclado[i])
      {
        printf("%i ", i);
      }
    }
    printf("\n");
    SDL_Delay(100);
  }

  // Limpiar pantalla
  drawSprite(10, 10, spriteA, 5);
  drawSprite(16, 10, spriteI, 5);
  drawSprite(28, 10, spriteO, 5);
  drawSprite(34, 10, spriteN, 5);
  drawSprite(22, 10, spriteM, 5);
  drawSprite(40, 10, spriteZ, 5);
  drawSprite(46, 10, spriteA, 5);
  drawSprite(52, 10, spriteS, 5);

  printPantalla(&pantalla);

  SDL_Delay(1000);

  // Dibujar DEIVID
  drawSprite(10, 10, spriteD, 5);
  drawSprite(16, 10, spriteE, 5);
  drawSprite(22, 10, spriteI, 5);
  drawSprite(28, 10, spriteV, 5);
  drawSprite(34, 10, spriteI, 5);
  drawSprite(40, 10, spriteD, 5);

  printPantalla(&pantalla);

  SDL_Init(SDL_INIT_AUDIO);
  iniciarSonido();

  // Bucle principal del emulador
  for (int i = 0; i < 1000; i++)
  {

    // Simular un ciclo de CPU (~60 veces por segundo)
    SDL_Delay(16);      // 16 ms = ~60 FPS
    decrement_timers(); // Decrementar los timers (delay y sonido)

    printf("ST: %i\t", get_sound_timer());
    printf("DT: %i\n", get_delay_timer());
    if (i % 200 == 0)
    {
      set_sound_timer(50);
      set_delay_timer(100);
    }
  }

  SDL_CloseAudio();

  cerrarPantalla(&pantalla);
  SDL_Quit();

  return 0;
}
