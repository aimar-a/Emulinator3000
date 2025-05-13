#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include "network_util.hpp"

// Constantes de configuración
constexpr int PORT = 8080;
constexpr const char *SERVER_IP = "127.0.0.1";

// Función principal del cliente
void client_run();

#endif // CLIENTE_H