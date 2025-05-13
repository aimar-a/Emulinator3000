#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include <stdint.h>
#include <dirent.h>

#include "bd.h"
#include "chip8_cpu.h"
#include "nes_cpu.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

void server_run();
void clienteAnonimo(socket_t client_socket);
void clienteConocido(socket_t client_socket, char *username);
void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount);
void servirChip8();
void servirNES();

#endif // SERVER_H