#ifndef NES_PPU_H
#define NES_PPU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint8_t memory[0x4000];               // 16 KB de memoria
  uint8_t palette[32];                  // Paleta de colores
  uint8_t screen[256 * 240];            // 256x240 pixels
  uint8_t oam[256];                     // Object Attribute Memory (256 entradas de 4 bytes)
  bool nmi_occurred;                    // Bandera de interrupción NMI
  bool nmi_output;                      // Salida de la interrupción NMI
  bool nmi_previous;                    // Estado previo de la interrupción NMI
  bool grayscale;                       // Modo de escala de grises
  bool show_bg;                         // Mostrar fondo
  bool show_sprites;                    // Mostrar sprites
  bool emphasize_r;                     // Énfasis en rojo
  bool emphasize_g;                     // Énfasis en verde
  bool emphasize_b;                     // Énfasis en azul
  uint16_t v;                           // Registro de dirección de VRAM
  uint16_t t;                           // Registro temporal de VRAM
  uint8_t x;                            // Registro X de fine scrolling
  uint8_t w;                            // Bandera de escritura
  uint8_t f;                            // Registro de estado
  uint8_t bg_next_tile_id;              // ID del siguiente tile de fondo
  uint8_t bg_next_tile_attrib;          // Atributo del siguiente tile de fondo
  uint8_t bg_next_tile_lsb;             // LSB del siguiente tile de fondo
  uint8_t bg_next_tile_msb;             // MSB del siguiente tile de fondo
  uint16_t bg_shifter_pattern_lo;       // Patrón de fondo (16 bits)
  uint16_t bg_shifter_pattern_hi;       // Patrón de fondo (16 bits)
  uint16_t bg_shifter_attrib_lo;        // Atributo de fondo (16 bits)
  uint16_t bg_shifter_attrib_hi;        // Atributo de fondo (16 bits)
  uint8_t bg_shifter_attrib_latch_lo;   // Atributo de fondo (8 bits)
  uint8_t bg_shifter_attrib_latch_hi;   // Atributo de fondo (8 bits)
  uint8_t sprite_count;                 // Cantidad de sprites encontrados
  uint8_t sprite_patterns[8];           // Patrones de sprites
  uint8_t sprite_positions[8];          // Posiciones de sprites
  uint8_t sprite_priorities[8];         // Prioridades de sprites
  uint8_t sprite_indexes[8];            // Índices de sprites
  uint8_t sprite_shifter_pattern_lo[8]; // Patrones de sprites (8 bits)
  uint8_t sprite_shifter_pattern_hi[8]; // Patrones de sprites (8 bits)
  bool sprite_zero_hit;                 // Bandera de colisión con sprite 0
  bool sprite_zero_being_rendered;      // Sprite 0 está siendo renderizado
  uint8_t oam_addr;                     // Dirección de OAM
  uint8_t primary_oam[64];              // OAM primario
  uint8_t secondary_oam[32];            // OAM secundario
  uint8_t sprite_ram_address;           // Dirección de RAM de sprites
  uint8_t sprite_ram_data;              // Datos de RAM de sprites
  uint8_t sprite_ram_data_buffer;       // Buffer de datos de RAM de sprites
  bool odd_frame;                       // Bandera de frame impar
} PPU;