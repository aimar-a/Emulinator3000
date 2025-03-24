#include "nes_cpu.h"

void nes_launch()
{
  NES *nes = (NES *)malloc(sizeof(NES));
  nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
  nes->ppu = (PPU *)malloc(sizeof(PPU));

  nes_reset(nes);
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
