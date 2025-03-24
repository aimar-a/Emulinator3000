#include "nes_ppu.h"

void ppu_main_loop(PPU *ppu, uint8_t *screen)
{
  printf("Scanline: %d\n", ppu->scanline);
  ppu->scanline++;
  // esto aqui deberia de hacer cosas pero no se el que :)
  if (ppu->scanline < 240)
  {
    // Renderizar scanline
    // render_scanline(ppu, screen);
  }
  else if (ppu->scanline == 240)
  {
    // No hacer nada
  }
  else if (ppu->scanline == 241)
  {
    // Iniciar VBlank
    ppu->status |= 0x80;
  }
  else if (ppu->scanline == 261)
  {
    // Terminar VBlank
    ppu->status &= ~0x80;
    ppu->scanline = 0;
  }
  else
  {
    // Renderizar scanline
    // render_scanline(ppu, screen);
  }
}