#include "network_util.hpp"
#include <stdexcept>
#include <vector>
#include <string.h>

namespace net
{
  socket_t connect(const ConnectionConfig &config)
  {
    // Crear el socket
    socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET_VALUE)
    {
      throw network_error("Socket creation failed", WSAGetLastError());
    }

// Configurar timeout (opcional)
#ifdef _WIN32
    DWORD timeout = config.timeout_ms;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = config.timeout_ms / 1000;
    tv.tv_usec = (config.timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv));
#endif

    // Resolver dirección del servidor
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config.port);

    // Convertir dirección IP
    if (inet_pton(AF_INET, config.host.c_str(), &server_addr.sin_addr) <= 0)
    {
      // Si no es una IP válida, intentar resolver como hostname
      struct hostent *host = gethostbyname(config.host.c_str());
      if (host == nullptr)
      {
        close_socket(sock);
        throw network_error("Host resolution failed", h_errno);
      }
      memcpy(&server_addr.sin_addr, host->h_addr_list[0], host->h_length);
    }

    // Establecer conexión
    if (::connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR_VALUE)
    {
      int error = WSAGetLastError();
      close_socket(sock);
      throw network_error("Connection failed", error);
    }

    return sock;
  }

  void send_data(socket_t sock, const void *data, size_t size)
  {
    // Primero enviar el tamaño (4 bytes en network byte order)
    uint32_t net_size = htonl(static_cast<uint32_t>(size));
    const char *size_ptr = reinterpret_cast<const char *>(&net_size);

    size_t total_sent = 0;
    while (total_sent < sizeof(net_size))
    {
      int sent = send(sock, size_ptr + total_sent, sizeof(net_size) - total_sent, 0);
      if (sent <= 0)
      {
        throw network_error("Error sending size", WSAGetLastError());
      }
      total_sent += sent;
    }

    // Luego enviar los datos
    const char *data_ptr = static_cast<const char *>(data);
    total_sent = 0;
    while (total_sent < size)
    {
      int sent = send(sock, data_ptr + total_sent, size - total_sent, 0);
      if (sent <= 0)
      {
        throw network_error("Error sending data", WSAGetLastError());
      }
      total_sent += sent;
    }
  }

  size_t receive_data(socket_t sock, void *buffer, size_t buffer_size)
  {
    // Recibir el tamaño primero
    uint32_t net_size;
    char *size_ptr = reinterpret_cast<char *>(&net_size);

    size_t total_received = 0;
    while (total_received < sizeof(net_size))
    {
      int recvd = recv(sock, size_ptr + total_received, sizeof(net_size) - total_received, 0);
      if (recvd <= 0)
      {
        throw network_error("Error receiving size", WSAGetLastError());
      }
      total_received += recvd;
    }

    size_t size = ntohl(net_size);
    if (size > buffer_size)
    {
      throw std::runtime_error("Receive buffer too small");
    }

    // Recibir los datos
    char *data_ptr = static_cast<char *>(buffer);
    total_received = 0;
    while (total_received < size)
    {
      int recvd = recv(sock, data_ptr + total_received, size - total_received, 0);
      if (recvd <= 0)
      {
        throw network_error("Error receiving data", WSAGetLastError());
      }
      total_received += recvd;
    }

    return size;
  }

  std::string receive_data(socket_t sock, size_t max_size)
  {
    // Recibir el tamaño primero
    uint32_t net_size;
    char *size_ptr = reinterpret_cast<char *>(&net_size);

    size_t total_received = 0;
    while (total_received < sizeof(net_size))
    {
      int recvd = recv(sock, size_ptr + total_received, sizeof(net_size) - total_received, 0);
      if (recvd <= 0)
      {
        throw network_error("Error receiving size", WSAGetLastError());
      }
      total_received += recvd;
    }

    size_t size = ntohl(net_size);
    if (size > max_size)
    {
      throw std::runtime_error("Requested receive size too large");
    }

    // Recibir los datos en un string
    std::string result;
    result.resize(size);

    total_received = 0;
    while (total_received < size)
    {
      int recvd = recv(sock, &result[total_received], size - total_received, 0);
      if (recvd <= 0)
      {
        throw network_error("Error receiving data", WSAGetLastError());
      }
      total_received += recvd;
    }

    return result;
  }

} // namespace net