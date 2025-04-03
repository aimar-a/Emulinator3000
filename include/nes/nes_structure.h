
#ifndef NES_STRUCTURE_H
#define NES_STRUCTURE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define NMI_VECTOR 0xFFFA
#define RESET_VECTOR 0xFFFC
#define IRQ_VECTOR 0xFFFE
#define BRK_VECTOR 0xFFFE

#define EXECUTION_TYPE 2

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
  uint8_t fineXScroll;  // Fine X scroll
  uint8_t write_toggle; // Toggle de escritura

  uint8_t buffer; // Buffer de datos (ni idea para que se usa)

  // Scanline
  uint16_t scanline; // Línea actual de la pantalla

  // Ciclo
  uint16_t cycle; // Ciclo actual de la PPU

  // Frame
  uint16_t frame; // Frame actual

  // Scanline Sprites
  uint8_t scanlineSprites[8];  // Sprites visibles en la línea actual
  uint8_t scanlineSpriteCount; // Contador de sprites visibles
} PPU;

typedef struct
{
  // Pulse channel 1
  struct
  {
    uint8_t duty_cycle;       // Duty cycle (0-3)
    uint8_t volume;           // Volume/envelope (0-15)
    bool constant_volume;     // Constant volume flag
    bool length_halt;         // Length counter halt
    bool sweep_enable;        // Sweep unit enable
    uint8_t sweep_shift;      // Sweep shift amount
    bool sweep_negate;        // Sweep negate flag
    uint8_t sweep_period;     // Sweep period
    uint16_t timer;           // Timer value
    uint16_t timer_reload;    // Timer reload value
    uint8_t length_counter;   // Length counter
    uint8_t envelope_counter; // Envelope counter
    uint8_t envelope_divider; // Envelope divider
    uint8_t sweep_divider;    // Sweep divider
    uint16_t sequencer;       // Waveform sequencer
    double phase;             // Phase accumulator
  } pulse1;

  // Pulse channel 2 (similar to pulse1)
  struct
  {
    uint8_t duty_cycle;
    uint8_t volume;
    bool constant_volume;
    bool length_halt;
    bool sweep_enable;
    uint8_t sweep_shift;
    bool sweep_negate;
    uint8_t sweep_period;
    uint16_t timer;
    uint16_t timer_reload;
    uint8_t length_counter;
    uint8_t envelope_counter;
    uint8_t envelope_divider;
    uint8_t sweep_divider;
    uint16_t sequencer;
    double phase;
  } pulse2;

  // Triangle channel
  struct
  {
    bool length_halt;
    uint8_t linear_counter;
    uint8_t linear_reload;
    bool linear_reload_flag;
    uint16_t timer;
    uint16_t timer_reload;
    uint8_t length_counter;
    uint16_t sequencer;
    double phase;
  } triangle;

  // Noise channel
  struct
  {
    uint8_t volume;
    bool constant_volume;
    bool length_halt;
    uint16_t shift_register;
    uint16_t timer;
    uint16_t timer_reload;
    uint8_t length_counter;
    uint8_t envelope_counter;
    uint8_t envelope_divider;
    uint8_t mode; // Noise mode flag
  } noise;

  // DMC channel (simplified)
  struct
  {
    bool irq_enable;
    bool loop;
    uint8_t rate;
    uint8_t output_level;
    uint16_t sample_address;
    uint16_t sample_length;
  } dmc;

  // Status/control registers
  bool pulse1_enable;
  bool pulse2_enable;
  bool triangle_enable;
  bool noise_enable;
  bool dmc_enable;
  bool frame_interrupt;

  // Frame counter
  uint8_t frame_counter_mode;
  uint8_t frame_counter;

  // Audio output
  SDL_AudioDeviceID audio_device;
  float audio_buffer[1024]; // Buffer for audio output
  size_t audio_buffer_pos;
} APU;

typedef struct
{
  uint8_t header[16];   // Cabecera iNES (16 bytes)
  uint8_t *prg_rom;     // PRG-ROM (Código del juego)
  uint8_t *chr_rom;     // CHR-ROM (Gráficos)
  uint8_t prg_size;     // Tamaño de PRG-ROM en bloques de 16 KB
  uint8_t chr_size;     // Tamaño de CHR-ROM en bloques de 8 KB
  uint8_t prg_ram_size; // Tamaño de PRG-RAM en bloques de 8 KB
  uint8_t chr_ram_size; // Tamaño de CHR-RAM en bloques de 8 KB
  uint8_t mirroring;    // Mirroring (horizontal o vertical)
  uint8_t has_battery;  // Si tiene batería para guardar
  uint8_t has_trainer;  // Si tiene trainer
  uint8_t four_screen;  // Si usa VRAM de 4 pantallas
  uint8_t mapper;       // ID del Mapper
} NES_ROM;

typedef struct
{
  // CPU
  uint8_t A, X, Y; // Registros
  uint16_t PC;     // Contador de programa
  uint8_t SP;      // Stack Pointer
  uint8_t P;       // Registro de estado

  int cycles;       // Ciclos de CPU
  int stall_cycles; // Ciclos de espera

  // PPU
  PPU *ppu;

  // APU
  APU *apu;

  // Memoria
  uint8_t memory[0x2000]; // 64KB de memoria (deberia de ser 0x10000 pero vamos a leer el programa desde la rom, sin cargarlo a memoria) (lo reduzco mas porque separamos en modulos)
  uint8_t sram[0x2000];
  NES_ROM *rom; // ROM cargada

  // Display
  uint8_t screen[256 * 240]; // 256x240 pixels

  // Input
  bool controller_strobe;      // Strobe
  uint8_t controller_shift[2]; // Shift
  uint8_t controller_state[2]; // Estado de los botones

  // NMI
  bool pending_nmi; // NMI pendiente
} NES;

#endif