#ifndef NETWORK_UTIL_H
#define NETWORK_UTIL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#define close_socket closesocket
typedef SOCKET socket_t;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#define close_socket close
typedef int socket_t;
#endif

// Función para enviar datos con longitud prefijada
bool sendData(socket_t sock, const void *data, size_t size);

// Función para recibir datos con longitud prefijada
bool receiveData(socket_t sock, void *buffer, size_t buffer_size, size_t *received_size);

#endif // NETWORK_UTIL_H