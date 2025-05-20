#include "chip8_display.h"

uint8_t *pantalla;

// Inicializa la pantalla
void chip8displaySetPantalla(uint8_t *pantalla_)
{
  pantalla = pantalla_;
}

// Limpia la pantalla
void chip8displayLimpiarPantalla()
{
  chip8_log("INFO: Limpiando pantalla...\n");
  for (int i = 0; i < SCREEN_WIDTH_CHIP8 * SCREEN_HEIGHT_CHIP8; i++)
  {
    pantalla[i] = 0;
  }
  // chip8displayPrintPantalla();
  chip8_log("INFO: Pantalla limpiada.\n");
}

// Dibuja un sprite en la pantalla
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height)
{
  chip8_log("INFO: Dibujando sprite en posición (%d, %d) con altura %d...\n", x, y, height);
  int n = 8; // por defecto en el modo chip8 el sprite tendra 8 pixeles de ancho
  uint8_t colision = 0;
  for (int i = 0; i < height; i++)
  {
    if (height == 16)
    { // si el parametro de height es 16 es decir esta en modo superchip8 el sprite tendra 16 pixeles de ancho
      n = 16;
    }
    for (int j = 0; j < n; j++)
    {
      if ((sprite[i] & (0x80 >> j)) != 0)
      {
        int pos = (x + j) % SCREEN_WIDTH_CHIP8 + ((y + i) % SCREEN_HEIGHT_CHIP8) * SCREEN_WIDTH_CHIP8;
        if (pantalla[pos] == 1)
        {
          colision = 1;
        }
        pantalla[pos] ^= 1;
      }
    }
  }
  chip8_log("INFO: Sprite dibujado. Colisión: %d\n", colision);
  return colision;
}