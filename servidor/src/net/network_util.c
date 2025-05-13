#include "network_util.h"
#include <string.h>

bool sendData(socket_t sock, const void *data, size_t size)
{
  // Primero enviar el tamaño (4 bytes en orden de red)
  uint32_t net_size = htonl((uint32_t)size);
  int sent = send(sock, (const char *)&net_size, sizeof(net_size), 0);
  if (sent != sizeof(net_size))
  {
    return false;
  }

  // Luego enviar los datos
  const char *ptr = (const char *)data;
  while (size > 0)
  {
    sent = send(sock, ptr, size, 0);
    if (sent <= 0)
    {
      return false;
    }
    ptr += sent;
    size -= sent;
  }
  return true;
}

bool receiveData(socket_t sock, void *buffer, size_t buffer_size, size_t *received_size)
{
  // Recibir el tamaño primero
  uint32_t net_size;
  int recvd = recv(sock, (char *)&net_size, sizeof(net_size), MSG_WAITALL);
  if (recvd != sizeof(net_size))
  {
    return false;
  }

  size_t size = ntohl(net_size);
  if (size > buffer_size)
  {
    return false; // Buffer demasiado pequeño
  }

  // Recibir los datos
  char *ptr = (char *)buffer;
  size_t remaining = size;
  while (remaining > 0)
  {
    recvd = recv(sock, ptr, remaining, 0);
    if (recvd <= 0)
    {
      return false;
    }
    ptr += recvd;
    remaining -= recvd;
  }

  if (received_size)
  {
    *received_size = size;
  }
  return true;
}