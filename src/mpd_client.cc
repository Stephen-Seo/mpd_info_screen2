#include "mpd_client.h"

// Local includes
#include "constants.h"
#include "helpers.h"

// Standard library includes
#include <cstring>
#include <vector>

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

bool MPDClient::needs_auth() const { return flags.test(5); }

void MPDClient::attempt_auth(std::string passwd) {
  if (!is_ok() || tcp_socket < 0) {
    return;
  }

  std::vector<char> vec;
  vec.push_back('p');
  vec.push_back('a');
  vec.push_back('s');
  vec.push_back('s');
  vec.push_back('w');
  vec.push_back('o');
  vec.push_back('r');
  vec.push_back('d');
  vec.push_back(' ');

  for (char c : passwd) {
    vec.push_back(c);
  }
  vec.push_back('\n');

  ssize_t write_ret = write(tcp_socket, vec.data(), vec.size());
  if (write_ret == static_cast<ssize_t>(vec.size())) {
    // Successful write, do nothing here.
  } else if (errno == EAGAIN) {
    // Re-attempt auth later.
    LOG_PRINT(level, LogLevel::VERBOSE, "VERBOSE: Re-attempt auth later.");
    return;
  } else {
    flags.set(0);
    LOG_PRINT(level, LogLevel::ERROR,
              "ERROR: Failed to auth with MPD! errno {}", errno);
    return;
  }

  uint8_t buf[1024];
  std::memset(buf, 0, 1024);
  ssize_t read_ret = read(tcp_socket, buf, 1024);
  if (read_ret > 1) {
    LOG_PRINT(level, LogLevel::VERBOSE, "{:.{}s}",
              reinterpret_cast<const char *>(buf), read_ret);
    if (buf[0] == 'O' && buf[1] == 'K') {
      // Success, clear "need auth" flag.
      flags.reset(5);
    }
  } else {
    flags.set(0);
    LOG_PRINT(level, LogLevel::ERROR,
              "ERROR: Failed to auth with MPD (check OK)!");
    return;
  }
}

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
      LOG_PRINT(level, LogLevel::ERROR, "Failed to create tcp socket: errno {}",
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

    LOG_PRINT(level, LogLevel::VERBOSE,
              "VERBOSE: host_ip: {:x}, host port: {:x}",
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

    uint8_t buf[1024];
    std::memset(buf, 0, 1024);
    ssize_t read_ret = read(tcp_socket, buf, 1024);
    if (read_ret > 0) {
      LOG_PRINT(level, LogLevel::VERBOSE, "{:.{}s}",
                reinterpret_cast<const char *>(buf), read_ret);
      if (buf[0] != 'O' || buf[1] != 'K') {
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR,
                  "ERROR: Failed to get initial OK from MPD!");
      }
    } else {
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to read initial OK from MPD!");
    }
  } else if (flags.test(5)) {
    // Do nothing, wait for authentication.
  } else if (flags.test(2) && !flags.test(3)) {
    if (!flags.test(4)) {
      LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Attempting status write...");
      ssize_t write_ret = write(tcp_socket, "status\n", 7);
      if (write_ret != 7) {
        LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to send \"status\"!");
        flags.set(0);
        return;
      }
      flags.set(4);
    }
    uint8_t buf[1024];
    std::memset(buf, 0, 1024);
    LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Attempting status read...");
    ssize_t read_ret = read(tcp_socket, buf, 1024);
    if (read_ret > 0) {
      flags.reset(4);
      LOG_PRINT(level, LogLevel::VERBOSE, "{:.{}s}",
                reinterpret_cast<const char *>(buf), read_ret);
      if (buf[read_ret - 3] == 'O' && buf[read_ret - 2] == 'K' &&
          buf[read_ret - 1] == '\n') {
        flags.set(3);
        flags.reset(5);
      } else if (buf[0] == 'A' && buf[1] == 'C' && buf[2] == 'K') {
        if (buf[5] == '4' && buf[6] == '@') {
          // Permission/Auth required
          flags.set(5);
          LOG_PRINT(level, LogLevel::WARNING, "WARNING: MPD requires auth!");
        } else {
          flags.set(0);
          LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to get MPD status!");
        }
      }
    } else if (errno != EAGAIN) {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to receive after sending status! errno {}",
                errno);
      flags.set(0);
      return;
    }

    if (!flags.test(4)) {
      LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Done with status.");
    }
  } else {
    if (!flags.test(4)) {
      LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Attempting ping write...");
      ssize_t write_ret = write(tcp_socket, "ping\n", 5);
      if (write_ret != 5) {
        LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to send \"ping\"!");
        flags.set(0);
        return;
      }
      flags.set(4);
    }
    uint8_t buf[1024];
    std::memset(buf, 0, 1024);
    LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Attempting ping read...");
    ssize_t read_ret = read(tcp_socket, buf, 1024);
    if (read_ret > 0) {
      flags.reset(4);
      LOG_PRINT(level, LogLevel::VERBOSE, "{:.{}s}",
                reinterpret_cast<const char *>(buf), read_ret);
      if (buf[0] == 'O' && buf[1] == 'K') {
        flags.set(2);
      }
    } else if (errno != EAGAIN) {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to receive after sending ping!");
      flags.set(0);
      return;
    }

    if (!flags.test(4)) {
      LOG_PRINT(level, LogLevel::DEBUG, "DEBUG: Done with ping.");
    }
  }
}
