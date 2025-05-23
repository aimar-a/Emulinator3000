#ifndef CHIP8_CLIENT_HPP
#define CHIP8_CLIENT_HPP

#include "chip8_display.hpp"
#include "chip8_input.hpp"
#include "chip8_audio.hpp"
#include "network_util.hpp"
#include <cstdint>

/**
 * @brief Función principal de emulación del cliente CHIP8
 * @param sock Socket de conexión con el servidor
 */
void emulate_chip8(socket_t sock);

#endif // CHIP8_CLIENT_HPP