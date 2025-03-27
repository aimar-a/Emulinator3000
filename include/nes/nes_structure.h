
#ifndef NES_STRUCTURE_H
#define NES_STRUCTURE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint8_t vram[0x4000]; // 16 KB de VRAM (aunque solo se usan 4 KB directamente)
  uint8_t oam[256];     // 256 Bytes para los sprites (Object Attribute Memory)
  uint8_t palette[64];  // 64 Bytes para la paleta de colores

  // Registros
  /*
PPUCTRL 	$2000 	VPHB SINN 	W 	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select / X and Y scroll bit 8 (NN)
PPUMASK 	$2001 	BGRs bMmG 	W 	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
PPUSTATUS 	$2002 	VSO- ---- 	R 	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
OAMADDR 	$2003 	AAAA AAAA 	W 	OAM read/write address
OAMDATA 	$2004 	DDDD DDDD 	RW 	OAM data read/write
PPUSCROLL 	$2005 	XXXX XXXX YYYY YYYY 	Wx2 	X and Y scroll bits 7-0 (two writes: X scroll, then Y scroll)
PPUADDR 	$2006 	..AA AAAA AAAA AAAA 	Wx2 	VRAM address (two writes: most significant byte, then least significant byte)
PPUDATA 	$2007 	DDDD DDDD 	RW 	VRAM data read/write
OAMDMA 	$4014 	AAAA AAAA 	W 	OAM DMA high address
  */
  uint8_t ctrl;     // PPUCTRL ($2000)
  uint8_t mask;     // PPUMASK ($2001)
  uint8_t status;   // PPUSTATUS ($2002)
  uint8_t oamaddr;  // OAMADDR ($2003)
  uint8_t oamdata;  // OAMDATA ($2004)
  uint16_t scroll;  // PPUSCROLL ($2005)
  bool scroll_high; // Flag para saber si se escribió el byte alto o bajo
  uint16_t addr;    // PPUADDR ($2006)
  bool addr_high;   // Flag para saber si se escribió el byte alto o bajo
  uint8_t data;     // PPUDATA ($2007)
  uint8_t dma;      // OAMDMA ($4014)
  // Copilot usa estas también (no se lo q hacen)
  uint16_t t;           // Temporal
  uint16_t v;           // VRAM
  uint8_t x;            // Fine X scroll
  uint8_t write_toggle; // Toggle de escritura

  uint8_t buffer; // Buffer de datos (ni idea para que se usa)

  // Scanline
  uint16_t scanline; // Línea actual de la pantalla

  // Ciclo
  uint16_t cycle; // Ciclo actual de la PPU

  // Frame
  uint16_t frame; // Frame actual
} PPU;

typedef struct
{
  uint8_t header[16]; // Cabecera iNES (16 bytes)
  uint8_t *prg_rom;   // PRG-ROM (Código del juego)
  uint8_t *chr_rom;   // CHR-ROM (Gráficos)
  uint8_t prg_size;   // Tamaño de PRG-ROM en bloques de 16 KB
  uint8_t chr_size;   // Tamaño de CHR-ROM en bloques de 8 KB
} NES_ROM;

typedef struct
{
  // CPU
  uint8_t A, X, Y; // Registros
  uint16_t PC;     // Contador de programa
  uint8_t SP;      // Stack Pointer
  uint8_t P;       // Registro de estado

  // PPU
  PPU *ppu;

  // Memoria
  uint8_t memory[0x8000]; // 64KB de memoria (deberia de ser 0x10000 pero vamos a leer el programa desde la rom, sin cargarlo a memoria)
  NES_ROM *rom;           // ROM cargada

  // Display
  uint8_t screen[256 * 240]; // 256x240 pixels

  // Input
  bool controller_strobe;      // Strobe
  uint8_t controller_shift[2]; // Shift
  uint8_t controller_state[2]; // Estado de los botones

  // Mappers
  uint8_t mapper_bank;    // Banco de memoria PRG
  uint8_t chr_bank;       // Banco de memoria CHR (para gráficos)
  bool interrupt_enable;  // Habilitar interrupciones (para algunos mappers)
  uint8_t mapper_state;   // Estado del mapper (se usa en MMC1, MMC3, etc.)
  uint8_t current_mapper; // Tipo de mapper actual (como Mapper 0, 1, 3, etc.)
} NES;

#endif