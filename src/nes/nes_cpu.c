#include "nes_cpu.h"

void nes_launch()
{
  NES *nes = (NES *)malloc(sizeof(NES));
  nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
  nes->ppu = (PPU *)malloc(sizeof(PPU));

  nes_reset(nes);
  // Paleta de colores 2C02
  nes->ppu->palette[0] = 0x00;  // $00
  nes->ppu->palette[1] = 0x01;  // $01
  nes->ppu->palette[2] = 0x02;  // $02
  nes->ppu->palette[3] = 0x03;  // $03
  nes->ppu->palette[4] = 0x04;  // $04
  nes->ppu->palette[5] = 0x05;  // $05
  nes->ppu->palette[6] = 0x06;  // $06
  nes->ppu->palette[7] = 0x07;  // $07
  nes->ppu->palette[8] = 0x08;  // $08
  nes->ppu->palette[9] = 0x09;  // $09
  nes->ppu->palette[10] = 0x0A; // $0A
  nes->ppu->palette[11] = 0x0B; // $0B
  nes->ppu->palette[12] = 0x0C; // $0C
  nes->ppu->palette[13] = 0x0D; // $0D
  nes->ppu->palette[14] = 0x0E; // $0E
  nes->ppu->palette[15] = 0x0F; // $0F

  nes->ppu->palette[16] = 0x10; // $10
  nes->ppu->palette[17] = 0x11; // $11
  nes->ppu->palette[18] = 0x12; // $12
  nes->ppu->palette[19] = 0x13; // $13
  nes->ppu->palette[20] = 0x14; // $14
  nes->ppu->palette[21] = 0x15; // $15
  nes->ppu->palette[22] = 0x16; // $16
  nes->ppu->palette[23] = 0x17; // $17
  nes->ppu->palette[24] = 0x18; // $18
  nes->ppu->palette[25] = 0x19; // $19
  nes->ppu->palette[26] = 0x1A; // $1A
  nes->ppu->palette[27] = 0x1B; // $1B
  nes->ppu->palette[28] = 0x1C; // $1C
  nes->ppu->palette[29] = 0x1D; // $1D
  nes->ppu->palette[30] = 0x1E; // $1E
  nes->ppu->palette[31] = 0x1F; // $1F

  nes->ppu->palette[32] = 0x20; // $20
  nes->ppu->palette[33] = 0x21; // $21
  nes->ppu->palette[34] = 0x22; // $22
  nes->ppu->palette[35] = 0x23; // $23
  nes->ppu->palette[36] = 0x24; // $24
  nes->ppu->palette[37] = 0x25; // $25
  nes->ppu->palette[38] = 0x26; // $26
  nes->ppu->palette[39] = 0x27; // $27
  nes->ppu->palette[40] = 0x28; // $28
  nes->ppu->palette[41] = 0x29; // $29
  nes->ppu->palette[42] = 0x2A; // $2A
  nes->ppu->palette[43] = 0x2B; // $2B
  nes->ppu->palette[44] = 0x2C; // $2C
  nes->ppu->palette[45] = 0x2D; // $2D
  nes->ppu->palette[46] = 0x2E; // $2E
  nes->ppu->palette[47] = 0x2F; // $2F

  nes->ppu->palette[48] = 0x30; // $30
  nes->ppu->palette[49] = 0x31; // $31
  nes->ppu->palette[50] = 0x32; // $32
  nes->ppu->palette[51] = 0x33; // $33
  nes->ppu->palette[52] = 0x34; // $34
  nes->ppu->palette[53] = 0x35; // $35
  nes->ppu->palette[54] = 0x36; // $36
  nes->ppu->palette[55] = 0x37; // $37
  nes->ppu->palette[56] = 0x38; // $38
  nes->ppu->palette[57] = 0x39; // $39
  nes->ppu->palette[58] = 0x3A; // $3A
  nes->ppu->palette[59] = 0x3B; // $3B
  nes->ppu->palette[60] = 0x3C; // $3C
  nes->ppu->palette[61] = 0x3D; // $3D
  nes->ppu->palette[62] = 0x3E; // $3E
  nes->ppu->palette[63] = 0x3F; // $3F

  nes_display_init(nes->screen);
  nes_load_rom(nes, "resources/nes-roms/Super_mario_brothers.nes");
  // nes_rom_to_memory(nes); no vamos a cargar la ROM a memoria porque solo da problemas
  nes->PC = nes_read(nes, nes->rom->prg_size - 0x6) | (nes_read(nes, nes->rom->prg_size - 0x5) << 8);
  /*
  No entiendo porque es 0x6 y 0x5, deberia de ser 0x4 y 0x3 segun mi logica
  Ejemplo de Super Mario Bros:
  7FF0: 07 60 A9 00 60 EE FF 1E 1E 1F 82 80 00 80 F0 FF
  se deben leer los valores 00 80 que apuntan a 0x8000 que es donde se inicia el programa
  */

  nes->ppu->scanline = 0;

  for (int i = 0x7F00; i < 0x8000; i++)
  {
    if (i % 16 == 0)
    {
      printf("\n%04X: ", i);
    }
    printf("%02X ", nes->rom->prg_rom[i]);
  }
  printf("\n\n");

  nes_run(nes);
}

void nes_reset(NES *nes)
{
  // CPU
  nes->A = 0;
  nes->X = 0;
  nes->Y = 0;
  nes->PC = 0;
  nes->SP = 0xFD;
  nes->P = 0x24;

  for (int i = 0; i < 0x8000; i++)
  {
    nes->memory[i] = 0;
  }

  nes->controller_strobe = false;
  for (int i = 0; i < 2; i++)
  {
    nes->controller_shift[i] = 0;
    nes->controller_state[i] = 0;
  }

  nes->mapper_bank = 0;
  nes->chr_bank = 0;
  nes->interrupt_enable = false;
  nes->mapper_state = 0;
  nes->current_mapper = 0;

  // PPU
  for (int i = 0; i < 0x4000; i++)
  {
    nes->ppu->vram[i] = 0;
  }
  for (int i = 0; i < 256; i++)
  {
    nes->ppu->oam[i] = 0;
  }
  for (int i = 0; i < 32; i++)
  {
    nes->ppu->palette[i] = 0;
  }
  nes->ppu->ctrl = 0;
  nes->ppu->mask = 0;
  nes->ppu->status = 0;
  nes->ppu->oamaddr = 0;
  nes->ppu->oamdata = 0;
  nes->ppu->scroll = 0;
  nes->ppu->addr = 0;
  nes->ppu->data = 0;
  nes->ppu->dma = 0;

  nes->ppu->scanline = 0;
}

void nes_run(NES *nes)
{
  int cont = 0;

  // El tiempo por ciclo de la CPU en milisegundos
  const float cpu_cycle_time_ms = 1000.0f / 1790000.0f; // 1.79 MHz
  const float ppu_cycle_time_ms = 1000.0f / 5370000.0f; // 5.37 MHz

  while (true)
  {
    int cycles = nes_evaluate_opcode(nes); // Calcula los ciclos de CPU para la instrucción actual

    // Tiempo de espera para simular el tiempo real de un ciclo de la CPU
    SDL_Delay(cpu_cycle_time_ms * cycles); // Espera el tiempo correspondiente a los ciclos de CPU

    // Actualiza los controladores
    nes_controller_update(nes);

    // Ejecuta los ciclos de la PPU correspondientes al número de ciclos de CPU
    for (int i = 0; i < cycles * 3; i++)
    {
      // Sincroniza el ciclo de la PPU
      ppu_step(nes);

      // Retraso para simular el tiempo real de un ciclo de la PPU
      SDL_Delay(ppu_cycle_time_ms); // Espera el tiempo correspondiente a un ciclo de la PPU
    }

    // Dibuja la pantalla
    nes_display_draw(nes->screen);

    cont++;
    if (cont == 3)
    {
      cont = 0;
    }
  }
}
