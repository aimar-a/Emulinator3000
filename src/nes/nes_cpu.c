#include "nes_cpu.h"

void nes_launch()
{
  nes_log_clear();
  nes_log("INFO: Launching NES\n");

  NES *nes = (NES *)malloc(sizeof(NES));
  if (!nes)
  {
    nes_log("ERROR: Failed to allocate memory for NES structure\n");
    return;
  }
  nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
  if (!nes->rom)
  {
    nes_log("ERROR: Failed to allocate memory for NES_ROM structure\n");
    free(nes);
    return;
  }
  nes->ppu = (PPU *)malloc(sizeof(PPU));
  if (!nes->ppu)
  {
    nes_log("ERROR: Failed to allocate memory for PPU structure\n");
    free(nes->rom);
    free(nes);
    return;
  }

  nes_log("INFO: NES structures allocated successfully\n");

  nes_reset(nes);
  nes_log("INFO: NES reset\n");

  // Paleta de colores 2C02
  for (int i = 0; i < 64; i++)
  {
    nes->ppu->palette[i] = i;
  }
  nes_log("INFO: PPU palette initialized\n");

  nes_display_init();
  nes_log("INFO: Display initialized\n");

  if (nes_load_rom(nes, "resources/nes-roms/Super_mario_brothers.nes"))
  // if (nes_load_rom(nes, "resources/nes-roms/Donkey Kong (World) (Rev A).nes"))
  //   if (nes_load_rom(nes, "resources/nes-roms/Duck Hunt (World).nes"))
  //   if (nes_load_rom(nes, "resources/nes-roms/Tennis (USA) (GameCube Edition).nes"))
  {
    nes_log("ERROR: Failed to load ROM\n");
    free(nes->ppu);
    free(nes->rom);
    free(nes);
    return;
  }
  nes_log("INFO: ROM loaded successfully\n");

  nes->PC = nes_read_address(nes, RESET_VECTOR);
  nes_log("INFO: Program Counter set to 0x%04X\n", nes->PC);

  nes->ppu->scanline = 0;

  for (int i = 0x0000; i < 0x4000; i++) // TODO: quitar esto cuando se acabe
  {
    if (i % 16 == 0)
    {
      printf("\n%04X: ", i);
    }
    printf("%02X ", nes->rom->prg_rom[i]);
  }
  printf("\n\n");

  nes_display_draw(nes->screen);

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

  nes->rom->prg_size = 0;
  nes->rom->chr_size = 0;
  nes->rom->prg_ram_size = 0;
  nes->rom->chr_ram_size = 0;
  nes->rom->mirroring = 0;
  nes->rom->has_battery = 0;
  nes->rom->has_trainer = 0;
  nes->rom->four_screen = 0;
  nes->rom->mapper = 0;
  nes->rom->prg_rom = NULL;
  nes->rom->chr_rom = NULL;

  // PPU
  for (int i = 0; i < 0x4000; i++)
  {
    nes->ppu->vram[i] = 0;
  }
  for (int i = 0; i < 256; i++)
  {
    nes->ppu->oam[i] = 0;
  }
  for (int i = 0; i < 64; i++)
  {
    nes->ppu->palette[i] = 0;
  }
  nes->ppu->ctrl = 0;
  nes->ppu->mask = 0;
  nes->ppu->status = 0;
  nes->ppu->oamaddr = 0;
  nes->ppu->oamdata = 0;
  nes->ppu->scroll = 0;
  nes->ppu->scroll_high = false;
  nes->ppu->addr = 0;
  nes->ppu->addr_high = false;
  nes->ppu->data = 0;
  nes->ppu->dma = 0;

  nes->ppu->scanline = 0;

  nes_log("INFO: NES reset completed\n");
}

void nes_run(NES *nes)
{
  int cont = 0;

  // El tiempo por ciclo de la CPU en milisegundos
  const float cpu_cycle_time_ms = 1000.0f / 1790000.0f; // 1.79 MHz
  const float ppu_cycle_time_ms = 1000.0f / 5370000.0f; // 5.37 MHz

  nes_log("INFO: Starting NES emulation\n");

  while (true)
  {
    // -------------------
    // Simulacion original
    // -------------------
    if (EXECUTION_TYPE == 0)
    {
      int cycles = cpu_step(nes); // Calcula los ciclos de CPU para la instrucción actual

      // Tiempo de espera para simular el tiempo real de un ciclo de la CPU
      // SDL_Delay(cpu_cycle_time_ms * cycles); // Espera el tiempo correspondiente a los ciclos de CPU

      // Ejecuta los ciclos de la PPU correspondientes al número de ciclos de CPU
      for (int i = 0; i < cycles * 3; i++)
      {
        // Sincroniza el ciclo de la PPU
        ppu_step(nes);

        // Retraso para simular el tiempo real de un ciclo de la PPU
        // SDL_Delay(ppu_cycle_time_ms); // Espera el tiempo correspondiente a un ciclo de la PPU
      }
    }
    // ---------------------
    // Simulacion optimizada
    // ---------------------
    else if (EXECUTION_TYPE == 1)
    {
      for (int i = 0; i < 4; i++)
      {
        cpu_step(nes); // Ejecuta un ciclo de la CPU
      }

      // log_check_ppu_ram(nes);
      ppu_step_optimized(nes); // Dibuja un frame de la pantalla
    }
    // ---------------------------
    // Simulacion original copiada
    // ---------------------------
    else if (EXECUTION_TYPE == 2)
    {
      int cycles = cpu_step(nes); // Calcula los ciclos de CPU para la instrucción actual

      // Ejecuta los ciclos de la PPU correspondientes al número de ciclos de CPU
      for (int i = 0; i < cycles * 3; i++)
      {
        // Sincroniza el ciclo de la PPU
        ppu_step_copy(nes);
      }
    }

    // Actualiza los controladores
    if (nes_controller_update(nes))
    {
      break;
    }
  }

  nes_log("INFO: NES emulation stopped\n");
  log_check_ppu_ram(nes);

  nes_display_destroy(nes->screen);
  free(nes->ppu);
  free(nes->rom);
  free(nes);
  nes_log("INFO: NES structures freed\n");
}

void log_check_ppu_ram(NES *nes)
{
  nes_log("\nINFO: PPU VRAM:");
  for (int i = 0x2000; i < 0x3000; i++)
  {
    if (i % 16 == 0)
    {
      nes_log("\n%04X: ", i);
    }
    nes_log("%02X ", nes->ppu->vram[i]);
  }
  nes_log("\n\n");

  nes_log("INFO: PPU Palette:");
  for (int i = 0; i < 64; i++)
  {
    if (i % 16 == 0)
    {
      nes_log("\n%02X: ", i);
    }
    nes_log("%02X ", nes->ppu->palette[i]);
  }
  nes_log("\n\n");

  nes_log("INFO: PPU OAM:");
  for (int i = 0; i < 256; i++)
  {
    if (i % 16 == 0)
    {
      nes_log("\n%02X: ", i);
    }
    nes_log("%02X ", nes->ppu->oam[i]);
  }
  nes_log("\n\n");
}

uint8_t cpu_step(NES *nes)
{
  if (nes->pending_nmi)
  {
    nes->pending_nmi = false;
    nes_push_address(nes, nes->PC);
    nes_push(nes, nes->P);
    nes->PC = nes_read_address(nes, NMI_VECTOR);
  }
  return nes_evaluate_opcode(nes);
}