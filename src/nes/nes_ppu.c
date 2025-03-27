#include "nes_ppu.h"

// Avanza un ciclo de la PPU de forma clasica
void ppu_step(NES *nes) // TODO: lo de los ciclos no esta bien (quizas hay que hacer primero los ciclos y dentro los scanlines?)
{                       // TODO: Implementar el resto de la PPU (registros y lo que hace cada uno, y creo q gestion de memoria y algo mas habra q hacer)
  nes->ppu->cycle++;

  // 1️⃣ Pre-Render (Scanline -1):
  // Ocurre una scanline antes de que comience el dibujo de la imagen.
  // La PPU prepara los registros internos y actualiza el desplazamiento del scroll.
  if (nes->ppu->scanline == 261)
  {
    if (nes->ppu->cycle == 1)
    {
      nes->ppu->status &= ~0x80;
      nes_log("INFO: Exiting VBlank\n");
      nes->ppu->scroll = 0; // TODO: nose que hay que asignar aqui
      nes->ppu->addr = 0;   // TODO: nose que hay que asignar aqui
    }
  }
  // 2️⃣ Renderizado Visible (Scanlines 0-239)
  // Durante estas 240 scanlines, la PPU genera la imagen en la pantalla.
  // Cada scanline tarda 341 ciclos de la PPU.
  // Se procesan tiles y sprites para cada línea, sacando píxeles de la memoria VRAM.
  else if (nes->ppu->scanline < 240)
  {
    if (nes->ppu->cycle == 1)
    {
      nes_log("INFO: Starting new scanline %d\n", nes->ppu->scanline);
      // Dibuja la pantalla
      nes_display_draw(nes->screen);
    }
    if (nes->ppu->cycle >= 1 && nes->ppu->cycle <= 256)
    {
      render_scanline(nes);
    }
  }
  // 3️⃣ VBlank (Scanlines 240-261)
  // Es la fase donde la PPU no dibuja nada en la pantalla.
  // La CPU puede escribir en la VRAM sin riesgo de corrupción.
  // La PPU activa el bit de VBlank en el registro $2002, indicando que la pantalla está lista para actualizarse.
  else if (nes->ppu->scanline < 261)
  {
    if (nes->ppu->scanline == 240)
    {
      if (nes->ppu->cycle == 1)
      {
        nes->ppu->status |= 0x80;
        nes_log("INFO: Entering VBlank\n");
        if (nes->ppu->ctrl & 0x80)
        {
          nes_log("INFO: NMI interrupt triggered\n");
          // TODO: Implementar interrupción NMI (ni idea de lo q es) (creo que deberia de hacerlo solo la cpu con sus instrucciones)
          // trigger_nmi(); // Generar interrupción si está habilitada
        }
      }
    }
  }

  if (nes->ppu->cycle >= 341)
  {
    nes->ppu->cycle = 0;
    nes->ppu->scanline++;

    if (nes->ppu->scanline > 261)
    {
      nes->ppu->scanline = 0;
      nes->ppu->frame++;
      nes_log("INFO: Frame: %d\n", nes->ppu->frame);
      printf("Frame: %d\n", nes->ppu->frame);
    }
    nes_log("INFO: Scanline: %d\n", nes->ppu->scanline);
  }
}

// Genera un frame de la pantalla de forma optimizada
void ppu_step_optimized(NES *nes)
{
  nes->ppu->status &= ~0x80;
  nes_log("INFO: Exiting VBlank\n");
  nes->ppu->scroll = 0; // TODO: nose que hay que asignar aqui
  nes->ppu->addr = 0;   // TODO: nose que hay que asignar aqui

  nes->ppu->scanline = 0;
  for (; nes->ppu->scanline < 240; nes->ppu->scanline++)
  {
    render_scanline(nes);
  }
  nes_display_draw(nes->screen);

  nes->ppu->frame++;
  nes_log("INFO: Frame: %d\n", nes->ppu->frame);

  nes->ppu->status |= 0x80;
  nes_log("INFO: Entering VBlank\n");
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
      nes->screen[nes->ppu->scanline * 256 + x] = sprite_palette;
    }
    else
    {
      nes->screen[nes->ppu->scanline * 256 + x] = bg_palette;
    }
  }
}

// Devuelve el color de un pixel del fondo
uint8_t get_background_pixel(NES *nes, int x, int y)
{
  if (!(nes->ppu->mask & 0x08))
    return 0;

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

// Devuelve el color de un pixel de un sprite
uint8_t get_sprite_pixel(NES *nes, int x, int y)
{
  if (!(nes->ppu->mask & 0x10))
    return 0;

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
        return color + 4;
    }
  }
  return 0;
}
