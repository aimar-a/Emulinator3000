#ifndef NES_CLIENT_HPP
#define NES_CLIENT_HPP
#include "nes_display.hpp"
#include "nes_controller.hpp"
#include "network_util.hpp"

void emulate_nes(socket_t sock);

#endif