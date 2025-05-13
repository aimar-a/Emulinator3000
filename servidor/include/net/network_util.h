#ifndef NETWORK_UTIL_H
#define NETWORK_UTIL_H

#include <stdint.h>
#include <stdbool.h>

// Definiciones específicas de plataforma
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket closesocket
#define sleep_ms(x) Sleep(x)
typedef SOCKET socket_t;
#define INVALID_SOCKET_VALUE INVALID_SOCKET
#define SOCKET_ERROR_VALUE SOCKET_ERROR
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/types.h>
#define close_socket close
#define sleep_ms(x) usleep(x * 1000)
typedef int socket_t;
#define INVALID_SOCKET_VALUE -1
#define SOCKET_ERROR_VALUE -1
#define WSAGetLastError() errno
#endif

// Función para enviar datos con longitud prefijada
bool sendData(socket_t sock, const void *data, size_t size);

// Función para recibir datos con longitud prefijada
bool receiveData(socket_t sock, void *buffer, size_t buffer_size, size_t *received_size);

#endif // NETWORK_UTIL_H