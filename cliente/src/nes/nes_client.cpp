#include "nes_client.hpp"

void emulate_nes(socket_t sock)
{
  nes_display_init();

  // Buffer para la pantalla
  uint8_t screen_buffer[SCREEN_WIDTH_NES * SCREEN_HEIGHT_NES * sizeof(uint8_t)];

  // Buffer para el controller
  uint8_t controllers[2];

  // Bucle principal de emulación
  while (1)
  {
    // Recibir señal de salida del servidor
    uint8_t exit_signal = 0;
    net::receive_data(sock, &exit_signal, sizeof(exit_signal));
    if (exit_signal == 0x01)
    {
      printf("INFO: Exit signal received from server\n");
      break;
    }

    // Recibir datos de la pantalla
    net::receive_data(sock, screen_buffer, SCREEN_WIDTH_NES * SCREEN_HEIGHT_NES * sizeof(uint8_t));

    // Dibujar en la pantalla
    nes_display_draw(screen_buffer);

    // Leer eventos de entrada
    if (nes_controller_update(controllers))
    {
      // Enviar señal de salida al servidor
      exit_signal = 0x01;
      net::send_data(sock, &exit_signal, sizeof(exit_signal));
      break;
    }
    else
    {
      // Enviar señal de continuar al servidor
      exit_signal = 0x00;
      net::send_data(sock, &exit_signal, sizeof(exit_signal));
    }

    // Enviar el estado del controlador al servidor
    net::send_data(sock, controllers, sizeof(uint8_t) * 2);

    // Controlar la velocidad de la emulación
    // SDL_Delay(16); // Aproximadamente 60 FPS
  }

  free(screen_buffer);
  nes_display_destroy();
}
