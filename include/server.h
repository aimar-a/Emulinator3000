#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock2.h>
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
void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount);
void servirChip8();
void servirNES();

#endif // SERVER_H