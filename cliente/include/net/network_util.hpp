#ifndef NETWORK_UTIL_HPP
#define NETWORK_UTIL_HPP

#include <cstdint>
#include <string>
#include <system_error>
#include <memory>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define close_socket closesocket
using socket_t = SOCKET;
constexpr socket_t INVALID_SOCKET_VALUE = INVALID_SOCKET;
constexpr int SOCKET_ERROR_VALUE = SOCKET_ERROR;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#define close_socket close
using socket_t = int;
constexpr socket_t INVALID_SOCKET_VALUE = -1;
constexpr int SOCKET_ERROR_VALUE = -1;
inline int WSAGetLastError() { return errno; }
#endif

namespace net
{
  // Estructura para configuración de conexión
  struct ConnectionConfig
  {
    std::string host;
    uint16_t port;
    int timeout_ms = 3000; // Timeout opcional
  };

  // Establece conexión con el servidor
  socket_t connect(const ConnectionConfig &config);

  // Excepción para errores de red
  class network_error : public std::system_error
  {
  public:
    network_error(const std::string &what, int error_code)
        : std::system_error(error_code, std::generic_category(), what) {}
  };

  // Envía datos con longitud prefijada (4 bytes big-endian)
  void send_data(socket_t sock, const void *data, size_t size);

  // Versión para std::string
  inline void send_data(socket_t sock, const std::string &str)
  {
    send_data(sock, str.data(), str.size());
  }

  // Recibe datos con longitud prefijada
  size_t receive_data(socket_t sock, void *buffer, size_t buffer_size);

  // Versión que devuelve std::string
  std::string receive_data(socket_t sock, size_t max_size = 4096);

} // namespace net

#endif // NETWORK_UTIL_HPP