#include "nes_ppu.h"

void ppu_step(NES *nes) // TODO: lo de los ciclos no esta bien (quizas hay que hacer primero los ciclos y dentro los scanlines?)
{                       // TODO: Implementar el resto de la PPU (registros y lo que hace cada uno, y creo q gestion de memoria y algo mas habra q hacer)
  // Avanzar ciclo de la PPU
  nes->ppu->cycle++;

  registers_operations(nes);

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
      render_scanline(nes);
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
        // TODO: Implementar interrupción NMI (ni idea de lo q es) (creo que deberia de hacerlo solo la cpu con sus instrucciones)
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

void render_scanline(NES *nes)
{
  for (int x = 0; x < 256; x++)
  {
    uint8_t bg_pixel = get_background_pixel(nes, x, nes->ppu->scanline);
    uint8_t sprite_pixel = get_sprite_pixel(nes, x, nes->ppu->scanline);

    uint8_t bg_palette = (bg_pixel == 0 ? 0 : nes->ppu->palette[bg_pixel]);
    uint8_t sprite_palette = (sprite_pixel == 0 ? 0 : nes->ppu->palette[sprite_pixel]);

    if (sprite_pixel && (nes->ppu->mask & 0x10))
    {
      nes->screen[nes->ppu->scanline * 256 + x] = sprite_palette; // Sprite
    }
    else
    {
      nes->screen[nes->ppu->scanline * 256 + x] = bg_palette; // Fondo
    }
  }
}

uint8_t get_background_pixel(NES *nes, int x, int y)
{
  if (!(nes->ppu->mask & 0x08))
    return 0; // Si el fondo está desactivado

  uint16_t base_nametable = 0x2000 | ((nes->ppu->ctrl & 0x03) * 0x400);
  uint16_t tile_x = x / 8;
  uint16_t tile_y = y / 8;

  uint16_t tile_index = nes->ppu->vram[base_nametable + tile_y * 32 + tile_x];
  uint16_t pattern_address = (nes->ppu->ctrl & 0x10 ? 0x1000 : 0x0000) + (tile_index * 16);

  uint8_t bitplane1 = nes->rom->chr_rom[pattern_address + (y % 8)];
  uint8_t bitplane2 = nes->rom->chr_rom[pattern_address + (y % 8) + 8];

  int bit = 7 - (x % 8);
  uint8_t color = ((bitplane1 >> bit) & 1) | (((bitplane2 >> bit) & 1) << 1);

  return color;
}

uint8_t get_sprite_pixel(NES *nes, int x, int y)
{
  if (!(nes->ppu->mask & 0x10))
    return 0; // Si los sprites están desactivados

  for (int i = 0; i < 64; i++)
  {
    Sprite *sprite = (Sprite *)&nes->ppu->oam[i * 4]; // TODO: esto esta fatal porque oam no es un array de sprites (es probable q haciendo esto se esté accediendo a memoria q no debería)
    if (x >= sprite->x && x < sprite->x + 8 && y >= sprite->y && y < sprite->y + 8)
    {
      int tile_index = sprite->tile;
      uint16_t pattern_address = (nes->ppu->ctrl & 0x08 ? 0x1000 : 0x0000) + (tile_index * 16);

      uint8_t bitplane1 = nes->rom->chr_rom[pattern_address + (y - sprite->y)];
      uint8_t bitplane2 = nes->rom->chr_rom[pattern_address + (y - sprite->y) + 8];

      int bit = 7 - (x - sprite->x);
      uint8_t color = ((bitplane1 >> bit) & 1) | (((bitplane2 >> bit) & 1) << 1);

      if (color)
        return color + 4; // Sprites usan una paleta diferente
    }
  }
  return 0;
}

// No se para que sirve :) (pero se usa)
void registers_operations(NES *nes)
{
  if (nes->ppu->cycle == 257)
  {
    nes->ppu->v = (nes->ppu->v & ~0x41F) | (nes->ppu->t & 0x41F);
  }
  else if (nes->ppu->cycle == 328 || nes->ppu->cycle == 336)
  {
    nes->ppu->v = (nes->ppu->v & ~0x7BE0) | (nes->ppu->t & 0x7BE0);
  }
  else if (nes->ppu->cycle == 260)
  {
    // Copiar horizontalmente
    nes->ppu->v = (nes->ppu->v & ~0x041F) | (nes->ppu->t & 0x041F);
  }
  else if (nes->ppu->cycle >= 280 && nes->ppu->cycle <= 304)
  {
    // Copiar verticalmente
    nes->ppu->v = (nes->ppu->v & ~0x7BE0) | (nes->ppu->t & 0x7BE0);
  }
}

// No se usa y no se para que sirve :)
void ppu_write(NES *nes, uint16_t address, uint8_t data)
{
  switch (address)
  {
  case 0x2000:
    nes->ppu->ctrl = data;
    nes->ppu->t = (nes->ppu->t & ~0xC00) | ((data & 0x03) << 10);
    break;
  case 0x2001:
    nes->ppu->mask = data;
    break;
  case 0x2003:
    nes->ppu->oamaddr = data;
    break;
  case 0x2004:
    nes->ppu->oam[nes->ppu->oamaddr++] = data;
    break;
  case 0x2005:
    if (nes->ppu->write_toggle == 0)
    {
      nes->ppu->t = (nes->ppu->t & ~0x1F) | (data >> 3);
      nes->ppu->x = data & 0x07;
      nes->ppu->write_toggle = 1;
    }
    else
    {
      nes->ppu->t = (nes->ppu->t & ~0x73E0) | ((data & 0xF8) << 2);
      nes->ppu->t = (nes->ppu->t & ~0x7000) | ((data & 0x07) << 12);
      nes->ppu->write_toggle = 0;
    }
    break;
  case 0x2006:
    if (nes->ppu->write_toggle == 0)
    {
      nes->ppu->t = (nes->ppu->t & ~0xFF00) | ((data & 0x3F) << 8);
      nes->ppu->write_toggle = 1;
    }
    else
    {
      nes->ppu->t = (nes->ppu->t & ~0x00FF) | data;
      nes->ppu->v = nes->ppu->t;
      nes->ppu->write_toggle = 0;
    }
    break;
  case 0x2007:
    nes->ppu->vram[nes->ppu->v++] = data;
    break;
  case 0x4014:
    nes->ppu->dma = data;
    break;
  }
}