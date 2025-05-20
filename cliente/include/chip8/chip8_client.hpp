#ifndef CHIP8_CLIENT_HPP
#define CHIP8_CLIENT_HPP
#include "chip8_display.hpp"
#include "chip8_input.hpp"
#include "network_util.hpp"

void emulate_chip8(socket_t sock);

#endif