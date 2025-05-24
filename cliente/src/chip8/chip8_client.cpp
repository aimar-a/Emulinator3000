#include "chip8_client.hpp"
#include "chip8_display.hpp"
#include "chip8_input.hpp"
#include "chip8_audio.hpp"
#include <vector>
#include <iostream>
#include <array>
#include <memory>

void emulate_chip8(socket_t sock)
{
  // Receive operation mode (CHIP8 or SuperCHIP8)
  uint8_t isSuperChip8 = 0;
  net::receive_data(sock, &isSuperChip8, sizeof(isSuperChip8));
  std::cout << "INFO: SuperChip8 mode: " << static_cast<int>(isSuperChip8) << std::endl;

  // Initialize display
  Chip8Display display;
  if (!display.initialize(isSuperChip8))
  {
    std::cerr << "ERROR: Failed to initialize display" << std::endl;
    return;
  }
  std::cout << "INFO: Display initialized successfully" << std::endl;

  // Initialize input handler
  Chip8Input inputHandler;

  // Initialize audio
  Chip8Audio audio;
  std::cout << "INFO: Audio initialized successfully" << std::endl;

  // Screen buffer
  std::vector<uint8_t> screen_buffer(SCREEN_WIDTH_CHIP8 * SCREEN_HEIGHT_CHIP8 * sizeof(uint32_t), 0);

  // Input buffer
  std::array<uint8_t, 16> keyboard_buffer{};

  // Main emulation loop
  while (1)
  {
    // Receive exit signal from server
    uint8_t exit_signal = 0;
    net::receive_data(sock, &exit_signal, sizeof(exit_signal));

    if (exit_signal == 0x01)
    {
      std::cout << "INFO: Exit signal received from server" << std::endl;
      break;
    }

    // Receive screen data
    net::receive_data(sock, screen_buffer.data(), screen_buffer.size());

    // Render screen
    display.render(screen_buffer.data());

    // Handle user input
    if (inputHandler.captureKeyboard(keyboard_buffer))
    {
      // Send exit signal to server
      exit_signal = 0x01;
      net::send_data(sock, &exit_signal, sizeof(exit_signal));
      break;
    }
    else
    {
      // Send continue signal
      exit_signal = 0x00;
      net::send_data(sock, &exit_signal, sizeof(exit_signal));
    }

    // Send keyboard state
    net::send_data(sock, keyboard_buffer.data(), keyboard_buffer.size());

    // Receive and update audio state
    uint8_t sound_timer = 0;
    net::receive_data(sock, &sound_timer, sizeof(sound_timer));
    audio.setSound(sound_timer);
  }

  // All cleanup is handled automatically by RAII
  std::cout << "INFO: Emulation loop ended, cleaning up resources..." << std::endl;
}