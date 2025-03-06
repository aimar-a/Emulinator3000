#include "chip8_display.h"

int main()
{
  uint8_t spriteA[5] = {0xF0, 0x90, 0xF0, 0x90, 0x90};
  uint8_t spriteI[5] = {0xF0, 0x20, 0x20, 0x20, 0xF0};
  uint8_t spriteM[5] = {0x90, 0xF0, 0x90, 0x90, 0x90};
  uint8_t spriteO[5] = {0xF0, 0x90, 0x90, 0x90, 0xF0};
  uint8_t spriteN[5] = {0xF0, 0x90, 0x90, 0x90, 0x90};
  uint8_t spriteZ[5] = {0xF0, 0x10, 0x20, 0x40, 0xF0};
  uint8_t spriteS[5] = {0xF0, 0x80, 0xF0, 0x10, 0xF0};

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

  SDL_Delay(5000);

  cerrarPantalla(&pantalla);

  return 0;
}
