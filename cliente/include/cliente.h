#ifndef CLIENTE_H
#define CLIENTE_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <stdio.h>

#include "menu_cmd_cpp.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SERVER_IP "127.0.0.1"

// Function declaration
void client_run();

#endif // CLIENTE_H
