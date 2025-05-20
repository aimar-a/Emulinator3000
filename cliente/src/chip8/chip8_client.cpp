#include "chip8_client.hpp"

void emulate_chip8(socket_t sock)
{
  printf("INFO: Emulando CHIP8...\n");
  uint8_t isSuperChip8 = 0;
  net::receive_data(sock, &isSuperChip8, sizeof(isSuperChip8));
  printf("INFO: Modo SuperChip8: %d\n", isSuperChip8);

  chip8displayInitPantalla(isSuperChip8);

  printf("INFO: Pantalla inicializada correctamente.\n");
  // Buffer para la pantalla
  uint8_t screen_buffer[SCREEN_WIDTH_CHIP8 * SCREEN_HEIGHT_CHIP8 * sizeof(uint32_t)];

  // Buffer para el input
  uint16_t keyboard = 0;

  printf("INFO: Emulando CHIP8...\n");

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
    net::receive_data(sock, screen_buffer, sizeof(screen_buffer));

    // Dibujar en la pantalla
    chip8displayPrintPantalla(screen_buffer);

    // Leer eventos de entrada
    if (chip8inputCapturarTeclado(&keyboard))
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
    net::send_data(sock, &keyboard, sizeof(keyboard));

    // Controlar la velocidad de la emulación
    // SDL_Delay(16); // Aproximadamente 60 FPS
  }

  chip8displayDestroyPantalla();
}
