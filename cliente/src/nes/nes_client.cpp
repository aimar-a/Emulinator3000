#include "nes_client.hpp"
#include <vector>
#include <array>
#include <iostream>
#include <memory>

void emulate_nes(socket_t sock)
{
  // Initialize display
  NesDisplay display;
  if (!display.initialize())
  {
    std::cerr << "ERROR: Failed to initialize NES display" << std::endl;
    return;
  }
  std::cout << "INFO: NES display initialized successfully" << std::endl;

  // Initialize controller
  NesController controller;

  // Initialize audio
  NesAudio audio;
  std::cout << "INFO: NES audio initialized successfully" << std::endl;

  // Screen buffer
  std::vector<uint8_t> screen_buffer(SCREEN_WIDTH_NES * SCREEN_HEIGHT_NES);

  // Controller states
  std::array<uint8_t, 2> controllers{};

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

    // Handle controller input
    if (controller.update(controllers))
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

    // Send controller states
    net::send_data(sock, controllers.data(), controllers.size());

    /*
    // Audio handling (commented out as per original)
    uint8_t queue_audio = 0;
    net::receive_data(sock, &queue_audio, sizeof(queue_audio));
    if (queue_audio) {
        std::array<float, BUFFER_SIZE> audio_buffer;
        net::receive_data(sock, audio_buffer.data(), audio_buffer.size() * sizeof(float));
        audio.queueAudio(audio_buffer);
    }
    */

    // Frame rate control (commented out as per original)
    // SDL_Delay(16); // Approximately 60 FPS
  }

  // All resources are automatically cleaned up by RAII
  std::cout << "INFO: NES emulation ended, cleaning up resources..." << std::endl;
}