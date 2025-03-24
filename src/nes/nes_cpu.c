#include "nes_cpu.h"

void nes_launch()
{
  NES *nes = (NES *)malloc(sizeof(NES));
  nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
  nes->ppu = (PPU *)malloc(sizeof(PPU));

  nes_reset(nes);
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
  nes->A = 0;
  nes->X = 0;
  nes->Y = 0;
  nes->PC = 0x8000;
  nes->SP = 0xFD;
  nes->P = 0x24;
  nes->controller_strobe = false;
  for (int i = 0; i < 2; i++)
  {
    nes->controller_shift[i] = 0;
    nes->controller_state[i] = 0;
  }
  for (int i = 0; i < 0x8000; i++)
  {
    nes->memory[i] = 0;
  }
}

void nes_run(NES *nes)
{
  int cont = 0;
  while (true)
  {
    SDL_Delay(8); // TODO: deberian de ser 0.186 ms en la version final
    if (cont == 0)
    {
      nes_evaluate_opcode(nes);
    }
    nes_controller_update(nes);
    ppu_main_loop(nes->ppu, nes->screen);
    nes_display_draw(nes->screen);

    cont++;
    if (cont == 2)
    {
      cont = 0;
    }
  }
}