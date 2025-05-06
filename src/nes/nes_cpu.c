#include "nes_cpu.h"

void nes_launch()
{
  nes_terminate = false;

  nes_log_clear();
  nes_log_instant("INFO: Launching NES\n");

  cargarConfiguracion("resources/config/config");

  NES *nes = (NES *)malloc(sizeof(NES));
  if (!nes)
  {
    nes_log_error("ERROR: Failed to allocate memory for NES structure\n");
    return;
  }
  nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
  if (!nes->rom)
  {
    nes_log_instant("ERROR: Failed to allocate memory for NES_ROM structure\n");
    free(nes);
    return;
  }
  nes->ppu = (PPU *)malloc(sizeof(PPU));
  if (!nes->ppu)
  {
    nes_log_error("ERROR: Failed to allocate memory for PPU structure\n");
    free(nes->rom);
    free(nes);
    return;
  }
  nes->apu = (APU *)malloc(sizeof(APU));
  if (!nes->apu)
  {
    nes_log_error("ERROR: Failed to allocate memory for APU structure\n");
    free(nes->ppu);
    free(nes->rom);
    free(nes);
    return;
  }

  nes_log_instant("INFO: NES structures allocated successfully\n");

  nes_reset(nes);
  nes_log_instant("INFO: NES reset\n");

  // Paleta de colores 2C02
  for (int i = 0; i < 64; i++)
  {
    nes->ppu->palette[i] = i;
  }
  nes_log_instant("INFO: PPU palette initialized\n");

  nes_display_init();
  nes_log_instant("INFO: Display initialized\n");

  // if (nes_load_rom(nes, "resources/nes-roms/Super_mario_brothers.nes"))
  // if (nes_load_rom(nes, "resources/nes-roms/Donkey Kong (World) (Rev A).nes"))
  // if (nes_load_rom(nes, "resources/nes-roms/Duck Hunt (World).nes"))
  if (nes_load_rom(nes, "resources/nes-roms/Tennis (USA) (GameCube Edition).nes"))
  {
    nes_log_error("ERROR: Failed to load ROM\n");
    free(nes->ppu);
    free(nes->rom);
    free(nes);
    return;
  }
  nes_log_instant("INFO: ROM loaded successfully\n");

  nes->PC = nes_read_address(nes, RESET_VECTOR);
  nes_log_instant("INFO: Program Counter set to 0x%04X\n", nes->PC);

  nes->ppu->scanline = 0;

  /*
  for (int i = 0x0000; i < 0x4000; i++) // TODO: quitar esto cuando se acabe
  {
    if (i % 16 == 0)
    {
      printf("\n%04X: ", i);
    }
    printf("%02X ", nes->rom->prg_rom[i]);
  }
  printf("\n\n");
  */

  nes_display_draw(nes->screen);

  nes_apu_init(nes->apu);

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

  for (int i = 0; i < 0x2000; i++)
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
  for (int i = 0; i < 32; i++)
  {
    nes->ppu->secondary_oam[i] = 0;
  }
  for (int i = 0; i < 256; i++)
  {
    nes->ppu->sprite_priority[i] = false;
  }
  nes->ppu->sprite0_visible = false;
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

  nes_log_instant("INFO: NES reset completed\n");
}

void nes_run(NES *nes)
{
  int cont = 0;

  // El tiempo por ciclo de la CPU en milisegundos
  const float cpu_cycle_time_ms = 1000.0f / 1790000.0f; // 1.79 MHz
  const float ppu_cycle_time_ms = 1000.0f / 5370000.0f; // 5.37 MHz

  nes_log_instant("INFO: Starting NES emulation\n");

  // Main emulation loop with accurate cycle timing
  while (1)
  {
    // Execute one CPU instruction and get consumed cycles
    int cpu_cycles = cpu_step(nes);

    // Run APU for each CPU cycle (APU runs at CPU clock rate)
    for (int i = 0; i < cpu_cycles; i++)
    {
      nes_apu_clock(nes->apu);
    }

    // Run PPU for 3 cycles per CPU cycle (PPU runs 3x faster than CPU)
    for (int i = 0; i < cpu_cycles * 3; i++)
    {
      ppu_step_simple(nes);
    }

    // Handle CPU stall cycles from DMA/other operations
    while (nes->stall_cycles > 0)
    {
      // Still need to run PPU/APU during stalls
      nes_apu_clock(nes->apu);
      for (int i = 0; i < 3; i++)
      {
        ppu_step_simple(nes);
      }
      nes->stall_cycles--;
      nes->cycles++;
    }

    // Update controllers
    if (nes_controller_update(nes))
    {
      break;
    }

    // Check for termination
    if (nes_terminate)
    {
      break;
    }

    // Optional: Add delay for speed control
    // uint32_t elapsed = SDL_GetTicks() - last_time;
    // if (elapsed < frame_time_ms) SDL_Delay(frame_time_ms - elapsed);
    // last_time = SDL_GetTicks();
  }

  nes_log_instant("\nINFO: Traceback:\n");
  nes_save_traceback();
  nes_log_instant("\n");

  nes_log_instant("INFO: NES emulation stopped\n");
  log_check_ppu_ram(nes);

  nes_display_destroy();
  nes_apu_cleanup(nes->apu);
  free(nes->apu);
  free(nes->ppu);
  free(nes->rom);
  free(nes);
  nes_log_instant("INFO: NES structures freed\n");
}

void log_check_ppu_ram(NES *nes)
{
  nes_log_instant("\nINFO: PPU REGISTERS:");
  nes_log_instant("\nPPUCTRL: 0x%02X", nes->ppu->ctrl);
  nes_log_instant("\nPPUMASK: 0x%02X", nes->ppu->mask);
  nes_log_instant("\nPPUSTATUS: 0x%02X", nes->ppu->status);
  nes_log_instant("\nOAMADDR: 0x%02X", nes->ppu->oamaddr);
  nes_log_instant("\nOAMDATA: 0x%02X", nes->ppu->oamdata);
  nes_log_instant("\nPPUSCROLL: 0x%02X", nes->ppu->scroll);
  nes_log_instant("\nPPUADDR: 0x%02X", nes->ppu->addr);
  nes_log_instant("\nPPUDATA: 0x%02X", nes->ppu->data);
  nes_log_instant("\nOAMDMA: 0x%02X", nes->ppu->dma);
  nes_log_instant("\n\n");

  nes_log_instant("\nINFO: PPU CHR ROM:");
  for (int i = 0; i < 0x2000; i++)
  {
    if (i % 16 == 0)
    {
      nes_log_instant("\n%04X: ", i);
    }
    nes_log_instant("%02X ", ppu_read_ram(nes, i));
  }
  nes_log_instant("\n\n");

  nes_log_instant("\nINFO: PPU VRAM:");
  for (int i = 0x2000; i < 0x3000; i++)
  {
    if (i % 16 == 0)
    {
      nes_log_instant("\n%04X: ", i);
    }
    nes_log_instant("%02X ", ppu_read_ram(nes, i));
  }
  nes_log_instant("\n\n");

  nes_log_instant("INFO: PPU Palette:");
  for (int i = 0; i < 0x20; i++)
  {
    if (i % 16 == 0)
    {
      nes_log_instant("\n3F%02X: ", i);
    }
    nes_log_instant("%02X ", ppu_read_ram(nes, 0x3F00 + i));
  }
  nes_log_instant("\n\n");

  nes_log_instant("INFO: PPU OAM:");
  for (int i = 0; i < 256; i++)
  {
    if (i % 16 == 0)
    {
      nes_log_instant("\n%02X: ", i);
    }
    nes_log_instant("%02X ", nes->ppu->oam[i]);
  }
  nes_log_instant("\n\n");

  nes_log_instant("INFO: SCREEN:");
  for (int i = 0; i < 256 * 240; i++)
  {
    if (i % 16 == 0)
    {
      nes_log_instant("\n%04X: ", i);
    }
    nes_log_instant("%02X ", nes->screen[i]);
  }
  nes_log_instant("\n\n");
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
  nes_evaluate_opcode(nes);
}