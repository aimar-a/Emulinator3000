#include "nes_ppu.h"

// CONSTANTS
// const int ScanlineCycleLength = 341;

const int ScanlineEndCycle = 340;
const int VisibleScanlines = 240;
const int ScanlineVisibleDots = 256;
const int FrameEndScanline = 261;

void ppu_step(NES *nes)
{
  // PreRender
  if (nes->ppu->scanline == 261)
  {
    if (nes->ppu->cycle == 1)
    {
      nes->ppu->status &= ~VBLANK_FLAG;
      nes->ppu->status &= ~SPRITE_0_HIT_FLAG;
    }
    else if (nes->ppu->cycle == ScanlineVisibleDots + 2 && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE))
    {
      // Set bits related to horizontal position
      nes->ppu->addr &= ~0x41f;              // Unset horizontal bits
      nes->ppu->addr |= nes->ppu->t & 0x41f; // Copy
    }
    else if (nes->ppu->cycle > 280 && nes->ppu->cycle <= 304 && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE))
    {
      // Set vertical bits
      nes->ppu->addr &= ~0x7be0;              // Unset bits related to horizontal
      nes->ppu->addr |= nes->ppu->t & 0x7be0; // Copy
    }
    //                 if (nes->ppu->cycle > 257 && nes->ppu->cycle < 320)
    //                     m_spriteDataAddress = 0;
    // if rendering is on, every other frame is one cycle shorter
    if (nes->ppu->cycle >= ScanlineEndCycle - (!(nes->ppu->frame % 2 == 0) && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE)))
    {
      // m_pipelineState = Render; // TODO: remplazar (creo que sobra)
      nes->ppu->cycle = nes->ppu->scanline = 0;
    }

    // add IRQ support for MMC3
    if (nes->ppu->cycle == 260 && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE))
    {
      nes_log_instant("WARNING: Scanline IRQ not implemented\n");
      //  TODO: implementar esto si hay tiempo (mappers)
      //  m_bus.scanlineIRQ();
    }
  }
  // Render
  else if (nes->ppu->scanline < 240)
  {
    if (nes->ppu->cycle > 0 && nes->ppu->cycle <= ScanlineVisibleDots)
    {
      uint8_t bgColor = 0, sprColor = 0;
      bool bgOpaque = false, sprOpaque = true;
      bool spriteForeground = false;

      int x = nes->ppu->cycle - 1;
      int y = nes->ppu->scanline;

      if ((nes->ppu->mask & BACKGROUND_ENABLE))
      {
        int x_fine = (nes->ppu->fineXScroll + x) % 8;
        if (nes->ppu->mask & BACKGROUND_LEFT_COLUMN_ENABLE || x >= 8)
        {
          // fetch tile
          uint16_t addr = 0x2000 | (nes->ppu->addr & 0x0FFF); // mask off fine y
          // uint16_t addr = 0x2000 + x / 8 + (y / 8) * (ScanlineVisibleDots / 8);
          uint8_t tile = ppu_read_ram(nes, addr);

          // fetch pattern
          // Each pattern occupies 16 bytes, so multiply by 16
          addr = (tile * 16) + ((nes->ppu->addr >> 12 /*y % 8*/) & 0x7); // Add fine y
          if (nes->ppu->ctrl & BACKGROUND_TILE_SELECT)
            addr += 0x1000; // set whether the pattern is in the high or low page
          // Get the corresponding bit determined by (8 - x_fine) from the right
          bgColor = (ppu_read_ram(nes, addr) >> (7 ^ x_fine)) & 1;             // bit 0 of palette entry
          bgColor |= ((ppu_read_ram(nes, addr + 8) >> (7 ^ x_fine)) & 1) << 1; // bit 1

          bgOpaque = bgColor; // flag used to calculate final pixel with the sprite pixel

          // fetch attribute and calculate higher two bits of palette
          addr = 0x23C0 | (nes->ppu->addr & 0x0C00) | ((nes->ppu->addr >> 4) & 0x38) | ((nes->ppu->addr >> 2) & 0x07);
          uint8_t attribute = ppu_read_ram(nes, addr);
          int shift = ((nes->ppu->addr >> 4) & 4) | (nes->ppu->addr & 2);
          // Extract and set the upper two bits for the color
          bgColor |= ((attribute >> shift) & 0x3) << 2;
        }
        // Increment/wrap coarse X
        if (x_fine == 7)
        {
          if ((nes->ppu->addr & 0x001F) == 31) // if coarse X == 31
          {
            nes->ppu->addr &= ~0x001F; // coarse X = 0
            nes->ppu->addr ^= 0x0400;  // switch horizontal nametable
          }
          else
          {
            nes->ppu->addr += 1; // increment coarse X
          }
        }
      }

      if ((nes->ppu->mask & SPRITE_ENABLE) && ((nes->ppu->mask & SPRITE_LEFT_COLUMN_ENABLE) || x >= 8))
      {
        // for (auto i : m_scanlineSprites) // TODO: reemplazar por un for normal
        for (int index = 0; index < nes->ppu->scanlineSpriteCount; index++)
        {
          int i = nes->ppu->scanlineSprites[index];
          uint8_t spr_x = nes->ppu->oam[i * 4 + 3];

          if (0 > x - spr_x || x - spr_x >= 8)
            continue;

          uint8_t spr_y = nes->ppu->oam[i * 4 + 0] + 1,
                  tile = nes->ppu->oam[i * 4 + 1],
                  attribute = nes->ppu->oam[i * 4 + 2];

          int length = (nes->ppu->ctrl & SPRITE_HEIGHT) ? 16 : 8;

          int x_shift = (x - spr_x) % 8, y_offset = (y - spr_y) % length;

          if ((attribute & 0x40) == 0) // If NOT flipping horizontally
            x_shift ^= 7;
          if ((attribute & 0x80) != 0) // IF flipping vertically
            y_offset ^= (length - 1);

          uint16_t addr = 0;

          if (!(nes->ppu->ctrl & SPRITE_HEIGHT))
          {
            addr = tile * 16 + y_offset;
            if (nes->ppu->ctrl & SPRITE_TILE_SELECT)
              addr += 0x1000;
          }
          else // 8x16 sprites
          {
            // bit-3 is one if it is the bottom tile of the sprite, multiply by two to get the next pattern
            y_offset = (y_offset & 7) | ((y_offset & 8) << 1);
            addr = (tile >> 1) * 32 + y_offset;
            addr |= (tile & 1) << 12; // Bank 0x1000 if bit-0 is high
          }

          sprColor |= (ppu_read_ram(nes, addr) >> (x_shift)) & 1;            // bit 0 of palette entry
          sprColor |= ((ppu_read_ram(nes, addr + 8) >> (x_shift)) & 1) << 1; // bit 1

          if (!(sprOpaque = sprColor))
          {
            sprColor = 0;
            continue;
          }

          sprColor |= 0x10;                   // Select sprite palette
          sprColor |= (attribute & 0x3) << 2; // bits 2-3

          spriteForeground = !(attribute & 0x20);

          // Sprite-0 hit detection
          if (!(nes->ppu->status & SPRITE_0_HIT_FLAG) && (nes->ppu->mask & BACKGROUND_ENABLE) && i == 0 && sprOpaque && bgOpaque)
          {
            nes->ppu->status |= SPRITE_0_HIT_FLAG;
          }

          break; // Exit the loop now since we've found the highest priority sprite
        }
      }

      uint8_t paletteAddr = bgColor;

      if ((!bgOpaque && sprOpaque) ||
          (bgOpaque && sprOpaque && spriteForeground))
        paletteAddr = sprColor;
      else if (!bgOpaque && !sprOpaque)
        paletteAddr = 0;
      // else bgColor

      nes->screen[x * 240 + y] = readPalette(nes, paletteAddr); // TODO
    }
    else if (nes->ppu->cycle == ScanlineVisibleDots + 1 && (nes->ppu->mask & BACKGROUND_ENABLE))
    {
      // Shamelessly copied from nesdev wiki
      if ((nes->ppu->addr & 0x7000) != 0x7000) // if fine Y < 7
        nes->ppu->addr += 0x1000;              // increment fine Y
      else
      {
        nes->ppu->addr &= ~0x7000;              // fine Y = 0
        int y = (nes->ppu->addr & 0x03E0) >> 5; // let y = coarse Y
        if (y == 29)
        {
          y = 0;                    // coarse Y = 0
          nes->ppu->addr ^= 0x0800; // switch vertical nametable
        }
        else if (y == 31)
          y = 0; // coarse Y = 0, nametable not switched
        else
          y += 1; // increment coarse Y
        nes->ppu->addr = (nes->ppu->addr & ~0x03E0) | (y << 5);
        // put coarse Y back into m_dataAddress
      }
    }
    else if (nes->ppu->cycle == ScanlineVisibleDots + 2 && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE))
    {
      // Copy bits related to horizontal position
      nes->ppu->addr &= ~0x41f;
      nes->ppu->addr |= nes->ppu->t & 0x41f;
    }

    //                 if (nes->ppu->cycle > 257 && nes->ppu->cycle < 320)
    //                     m_spriteDataAddress = 0;

    // add IRQ support for MMC3
    if (nes->ppu->cycle == 260 && (nes->ppu->mask & BACKGROUND_ENABLE) && (nes->ppu->mask & SPRITE_ENABLE))
    {
      nes_log_instant("WARNING: Scanline IRQ not implemented\n");
      //  TODO: implementar esto si hay tiempo (mappers)
      //  m_bus.scanlineIRQ();
    }

    if (nes->ppu->cycle >= ScanlineEndCycle)
    {
      // Find and index sprites that are on the next Scanline
      // This isn't where/when this indexing, actually copying in 2C02 is done
      // but (I think) it shouldn't hurt any games if this is done here

      nes->ppu->scanlineSpriteCount = 0;

      int range = 8;
      if ((nes->ppu->ctrl & SPRITE_HEIGHT))
      {
        range = 16;
      }

      size_t j = 0;
      for (size_t i = (nes->ppu->oamaddr) / 4; i < 64; ++i)
      {
        int diff = (nes->ppu->scanline - nes->ppu->oam[i * 4]);
        if (0 <= diff && diff < range)
        {
          if (j >= 8)
          {
            nes->ppu->status |= SPRITE_OVERFLOW_FLAG;
            break;
          }
          if (nes->ppu->scanlineSpriteCount >= 8)
          {
            nes_log_error("ERROR: Too many sprites on scanline %d\n", nes->ppu->scanline);
          }
          nes->ppu->scanlineSprites[j] = i;
          nes->ppu->scanlineSpriteCount++;
          ++j;
        }
      }

      ++nes->ppu->scanline;
      nes->ppu->cycle = 0;
    }

    if (nes->ppu->scanline >= VisibleScanlines)
    {
      // m_pipelineState = PostRender; // TODO: remplazar (creo que sobra)
    }
  }
  // PostRender
  if (nes->ppu->scanline == VisibleScanlines)
  {
    if (nes->ppu->cycle >= ScanlineEndCycle)
    {
      ++nes->ppu->scanline;
      nes->ppu->cycle = 0;
      // m_pipelineState = VerticalBlank; // TODO: remplazar (creo que sobra)

      // for (size_t x = 0; x < 256; ++x)
      //{
      //   for (size_t y = 0; y < 240; ++y)
      //   {
      //     m_screen.setPixel(x, y, nes->ppu->picture_buffer[x][y]);
      //   }
      // }
      nes_display_draw(nes->screen);
    }
  }
  // VerticalBlank
  else if (nes->ppu->scanline < FrameEndScanline)
  {
    if (nes->ppu->cycle == 1 && nes->ppu->scanline == VisibleScanlines + 1)
    {
      nes->ppu->status |= VBLANK_FLAG;
      if (nes->ppu->ctrl & NMI_ENABLE)
        nes->pending_nmi = true; // TODO: Implementar interrupción NMI
    }

    if (nes->ppu->cycle >= ScanlineEndCycle)
    {
      ++nes->ppu->scanline;
      nes->ppu->cycle = 0;
    }

    if (nes->ppu->scanline >= FrameEndScanline)
    {
      // m_pipelineState = PreRender; // TODO: remplazar (creo que sobra)
      nes->ppu->scanline = 0;
      nes->ppu->frame++;
      nes_log_traceback("INFO: Frame: %d\n", nes->ppu->frame);
    }
  }
  else
  {
    nes_log_error("ERROR: Scanline out of bounds: %d\n", nes->ppu->scanline);
  }
  ++nes->ppu->cycle;
}

void ppu_step_simple(NES *nes)
{
  // Increment PPU cycle counter
  nes->ppu->cycle++;

  if (nes->ppu->cycle == 1)
  {
    nes_log_traceback("INFO: PPU frame: %d, scanline: %d\n", nes->ppu->frame, nes->ppu->scanline);
  }

  // Handle end of scanline
  if (nes->ppu->cycle >= 341)
  {
    nes->ppu->cycle = 0;
    nes->ppu->scanline++;
  }

  // Visible scanlines (0-239)
  if (nes->ppu->scanline < 240)
  {
    // Visible cycles (1-256)
    if (nes->ppu->cycle >= 1 && nes->ppu->cycle <= 256)
    {
      // Background rendering
      if (nes->ppu->mask & BACKGROUND_ENABLE)
      {
        render_background_pixel(nes);
      }

      // Sprite rendering
      if (nes->ppu->mask & SPRITE_ENABLE)
      {
        // render_sprite_pixel(nes->ppu);
      }
    }

    // Fetch next tile data during cycles 1-256
    if (nes->ppu->cycle >= 1 && nes->ppu->cycle <= 256)
    {
      fetch_background_data(nes->ppu);
    }
  }
  // Post-render scanline (240)
  else if (nes->ppu->scanline == 240)
  {
    // Do nothing (idle)
  }
  // VBlank scanlines (241-260)
  else if (nes->ppu->scanline == 241 && nes->ppu->cycle == 1)
  {
    // Set VBlank flag
    nes->ppu->status |= VBLANK_FLAG;

    // Trigger NMI if enabled
    if (nes->ppu->ctrl & NMI_ENABLE)
    {
      nes->pending_nmi = true;
    }
  }

  // Pre-render scanline (261)
  if (nes->ppu->scanline == 261)
  {
    // Clear VBlank flag at end of VBlank
    if (nes->ppu->cycle == 1)
    {
      nes->ppu->scanline = 0;
      nes->ppu->status &= ~0x80;
      nes->ppu->frame++;
      nes_display_draw(nes->screen);
    }
  }
}

void render_background_pixel(NES *nes)
{
  uint16_t x = nes->ppu->cycle - 1;
  uint16_t y = nes->ppu->scanline;

  // Get nametable address (0x2000-0x2FFF)
  uint16_t nametable_addr = 0x2000 | (nes->ppu->addr & 0x0FFF);
  uint8_t tile_index = ppu_read_ram(nes, nametable_addr);

  // Get pattern table address (depends on PPUCTRL)
  uint16_t pattern_addr = (nes->ppu->ctrl & 0x10) ? 0x1000 : 0x0000;
  pattern_addr += tile_index * 16;

  // Get fine Y scroll (0-7)
  uint8_t fine_y = (nes->ppu->addr >> 12) & 0x7;

  // Get tile data
  uint8_t pattern_lsb = ppu_read_ram(nes, pattern_addr + fine_y);
  uint8_t pattern_msb = ppu_read_ram(nes, pattern_addr + fine_y + 8);

  // Get palette index (2 bits)
  uint8_t bit_pos = 7 - (x % 8);
  uint8_t palette_index = ((pattern_msb >> bit_pos) & 1) << 1 |
                          ((pattern_lsb >> bit_pos) & 1);

  // Get palette color
  uint8_t palette_addr = 0x3F00; // Background palette
  uint8_t color = nes->ppu->palette[palette_addr + palette_index];

  // Write to framebuffer
  nes->screen[y * 256 + x] = color;
}

void fetch_background_data(PPU *ppu)
{
  // Simplified - actual PPU has several fetch cycles
  if (ppu->cycle % 8 == 0)
  {
    // Increment coarse X scroll
    if ((ppu->addr & 0x001F) == 31)
    {
      ppu->addr &= ~0x001F; // Wrap around
      ppu->addr ^= 0x0400;  // Switch nametable
    }
    else
    {
      ppu->addr++;
    }
  }

  // Increment Y at end of scanline
  if (ppu->cycle == 256)
  {
    if ((ppu->addr & 0x7000) != 0x7000)
    {
      ppu->addr += 0x1000; // Increment fine Y
    }
    else
    {
      ppu->addr &= ~0x7000;
      uint16_t coarse_y = (ppu->addr & 0x03E0) >> 5;
      if (coarse_y == 29)
      {
        coarse_y = 0;
        ppu->addr ^= 0x0800; // Switch nametable
      }
      else if (coarse_y == 31)
      {
        coarse_y = 0;
      }
      else
      {
        coarse_y++;
      }
      ppu->addr = (ppu->addr & ~0x03E0) | (coarse_y << 5);
    }
  }
}

// Helper function to convert NES color to RGB
uint32_t nes_color_to_rgb(NES *nes, uint8_t nes_color)
{
  // NES colors are stored in a specific palette, convert to RGB here
  // This is a simplified example, actual conversion may vary
  uint32_t rgb = nes->ppu->palette[nes_color];
  return (rgb & 0xFF0000) | ((rgb & 0x00FF00) >> 8) | ((rgb & 0x0000FF) << 8);
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

  uint16_t tile_index = ppu_read_ram(nes, base_nametable + (tile_y * 32) + tile_x);
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

// Devuelve el color de un pixel de la paleta
uint8_t readPalette(NES *nes, uint8_t addr)
{
  if (addr >= 0x3F00 && addr < 0x3F20)
  {
    nes_log_error("ERROR: implementar el otro tipo de acceso a memoria readPalette\n");
  }
  if (addr >= 0x10 && addr % 4 == 0)
  {
    addr = addr & 0xf;
  }
  return nes->ppu->palette[addr];
}
