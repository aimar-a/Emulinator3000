#include "nes_ppu.h"

void ppu_step(NES *nes)
{
  // Avanzar ciclo de la PPU
  nes->ppu->cycle++;

  if (nes->ppu->scanline < 240)
  {
    // Renderizar scanline si estamos en el área visible
    if (nes->ppu->cycle == 1)
    {
      // Inicio de una nueva línea, resetear variables si es necesario
    }
    if (nes->ppu->cycle >= 1 && nes->ppu->cycle <= 256)
    {
      // Dibujar píxeles en la pantalla
      render_scanline(nes->ppu, nes->screen, nes->ppu->scanline);
    }
  }
  else if (nes->ppu->scanline == 240)
  {
    // Inicio de VBlank
    if (nes->ppu->cycle == 1)
    {
      nes->ppu->status |= 0x80; // Flag de VBlank
      if (nes->ppu->ctrl & 0x80)
      {
        // TODO: Implementar interrupción NMI (ni idea de lo q es)
        // trigger_nmi(); // Generar interrupción si está habilitada
      }
    }
  }
  else if (nes->ppu->scanline == 261)
  {
    // Scanline de pre-renderizado
    if (nes->ppu->cycle == 1)
    {
      nes->ppu->status &= ~0x80; // Borrar flag de VBlank
    }
  }

  // Fin del ciclo de la scanline
  if (nes->ppu->cycle >= 341)
  {
    nes->ppu->cycle = 0;
    nes->ppu->scanline++;

    if (nes->ppu->scanline > 261)
    {
      nes->ppu->scanline = 0; // Iniciar nuevo cuadro
    }
    printf("Scanline: %d\n", nes->ppu->scanline);
  }
}

void render_scanline(PPU *ppu, uint8_t *screen, uint16_t scanline)
{
  // TODO: Implementar renderizado de scanline
}