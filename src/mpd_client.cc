#include "mpd_client.h"

// Local includes
#include "constants.h"
#include "helpers.h"

// Standard library includes
#include <cstring>

// Unix includes
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

MPDClient::MPDClient(std::string host_ip, uint16_t host_port, LogLevel level)
    : flags(),
      events(),
      level(level),
      host_ip_value(helper_ipv4_str_to_value(host_ip)),
      host_port(host_port),
      tcp_socket(-1) {
  if (!host_ip_value.has_value()) {
    LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to parse ipv4 \"{}\"!",
              host_ip);
    flags.set(0);
  } else {
    flags.set(1);
  }
}

MPDClient::~MPDClient() {
  if (tcp_socket >= 0) {
    close(tcp_socket);
  }
}

MPDClient::MPDClient(MPDClient &&other)
    : flags(std::move(other.flags)),
      events(std::move(other.events)),
      level(std::move(other.level)),
      host_ip_value(std::move(other.host_ip_value)),
      host_port(other.host_port),
      tcp_socket(other.tcp_socket) {
  other.tcp_socket = -1;
}

MPDClient &MPDClient::operator=(MPDClient &&other) {
  this->flags = std::move(other.flags);
  this->events = std::move(other.events);
  this->level = std::move(other.level);
  this->host_ip_value = std::move(other.host_ip_value);
  this->host_port = other.host_port;
  this->tcp_socket = other.tcp_socket;
  other.tcp_socket = -1;

  return *this;
}

void MPDClient::reset_connection() {
  flags.reset(0);
  flags.set(1);
  if (tcp_socket >= 0) {
    close(tcp_socket);
    tcp_socket = -1;
  }
}

bool MPDClient::is_ok() const { return !flags.test(0); }

void MPDClient::update() {
  if (flags.test(0)) {
    return;
  } else if (flags.test(1)) {
    flags.reset(1);

    // Initialize connection.
    if (tcp_socket >= 0) {
      close(tcp_socket);
      tcp_socket = -1;
    }

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR, "Failed to create tpc socket: errno {}",
                errno);
      return;
    }

    struct sockaddr_in ipv4_sockaddr;
    std::memset(&ipv4_sockaddr, 0, sizeof(struct sockaddr_in));
    ipv4_sockaddr.sin_family = AF_INET;
    if (helper_is_big_endian()) {
      ipv4_sockaddr.sin_port = host_port;
    } else {
      ipv4_sockaddr.sin_port = htons(host_port);
    }
    ipv4_sockaddr.sin_addr.s_addr = host_ip_value.value();

    LOG_PRINT(level, LogLevel::VERBOSE, "host_ip: {:x}, host port: {:x}",
              ipv4_sockaddr.sin_addr.s_addr, ipv4_sockaddr.sin_port);

    int connect_ret = connect(
        tcp_socket, reinterpret_cast<const struct sockaddr *>(&ipv4_sockaddr),
        sizeof(struct sockaddr_in));
    if (connect_ret != 0) {
      close(tcp_socket);
      tcp_socket = -1;
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to connect to host! errno {}", errno);
      return;
    }
  } else if (flags.test(2) && !flags.test(3)) {
    LOG_PRINT(level, LogLevel::DEBUG, "Attempting status write...");
    ssize_t write_ret = write(tcp_socket, "CMD status\n", 9);
    if (write_ret != 9) {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to send \"CMD status\"!");
      flags.set(0);
      return;
    }
    uint8_t buf[1024];
    LOG_PRINT(level, LogLevel::DEBUG, "Attempting status read...");
    ssize_t read_ret = read(tcp_socket, buf, 1024);
    if (read_ret > 0) {
      std::println("{:.{}s}", reinterpret_cast<const char *>(buf), read_ret);
      if (buf[0] == 'O' && buf[1] == 'K') {
        flags.set(3);
      }
    } else {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to receive after sending status! errno {}",
                errno);
      flags.set(0);
      return;
    }
    LOG_PRINT(level, LogLevel::DEBUG, "Done with status.");
  } else {
    LOG_PRINT(level, LogLevel::DEBUG, "Attempting ping write...");
    ssize_t write_ret = write(tcp_socket, "CMD ping\n", 9);
    if (write_ret != 9) {
      LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to send \"CMD ping\"!");
      flags.set(0);
      return;
    }
    uint8_t buf[1024];
    LOG_PRINT(level, LogLevel::DEBUG, "Attempting ping read...");
    ssize_t read_ret = read(tcp_socket, buf, 1024);
    if (read_ret > 0) {
      std::println("{:.{}s}", reinterpret_cast<const char *>(buf), read_ret);
      if (buf[0] == 'O' && buf[1] == 'K') {
        flags.set(2);
      }
    } else {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to receive after sending ping!");
      flags.set(0);
      return;
    }
    LOG_PRINT(level, LogLevel::DEBUG, "Done with ping.");
  }
}
