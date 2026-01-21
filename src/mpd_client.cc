#include "mpd_client.h"

// Local includes
#include "constants.h"
#include "helpers.h"

// Standard library includes
#include <cstring>
#include <thread>
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

MPDClient::~MPDClient() { cleanup_close_tcp(); }

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
  cleanup_close_tcp();
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

  uint8_t buf[READ_BUF_SIZE];
  std::memset(buf, 0, READ_BUF_SIZE);
  ssize_t read_ret = read(tcp_socket, buf, READ_BUF_SIZE);
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
    cleanup_close_tcp();

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
      cleanup_close_tcp();
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to connect to host! errno {}", errno);
      return;
    }

    flags.set(4);
    auto [status, buf] = write_read("");
    if (flags.test(0)) {
      return;
    } else if (!buf.empty()) {
      if (buf[0] == 'O' && buf[1] == 'K') {
        // Successful, do nothing here.
      } else {
        cleanup_close_tcp();
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR,
                  "ERROR: Failed to get initial OK from MPD!");
        return;
      }
    } else {
      cleanup_close_tcp();
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to read initial OK from MPD!");
      return;
    }

    // Set the max binary size:
    bool binary_size_set = false;
    do {
      std::tie(status, buf) = write_read("binarylimit 4000\n");
      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (buf[0] == 'O' && buf[1] == 'K') {
          // Success.
          binary_size_set = true;
          continue;
        } else {
          cleanup_close_tcp();
          flags.set(0);
          LOG_PRINT(level, LogLevel::ERROR,
                    "ERROR: Failed to set \"binarylimit\", no OK!");
          return;
        }
      } else {
        cleanup_close_tcp();
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR,
                  "ERROR: Failed to set \"binarylimit\"!");
        return;
      }

      std::this_thread::sleep_for(LOOP_SLEEP_TIME);
    } while (!binary_size_set);
  } else if (flags.test(5)) {
    // Do nothing, wait for authentication.
  } else if (!flags.test(2)) {
    // Do ping.
    bool successful_write_read = false;
    do {
      auto [status, buf] = write_read("ping\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (buf[0] == 'O' && buf[1] == 'K') {
          // Success
          successful_write_read = true;
          flags.set(2);
          continue;
        } else {
          cleanup_close_tcp();
          flags.set(0);
          LOG_PRINT(level, LogLevel::ERROR,
                    "ERROR: Failed to ping MPD (no OK)!");
          return;
        }
      } else {
        cleanup_close_tcp();
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to ping MPD!");
        return;
      }

      std::this_thread::sleep_for(LOOP_SLEEP_TIME);
    } while (!successful_write_read);
  } else if (!flags.test(3)) {
    // Do "status".
    bool successful_write_read = false;
    do {
      auto [status, buf] = write_read("status\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (buf.at(buf.size() - 3) == 'O' && buf.at(buf.size() - 2) == 'K' &&
            buf.at(buf.size() - 1) == '\n') {
          // Success
          flags.set(3);
          successful_write_read = true;
          continue;
        } else if (buf.at(0) == 'A' && buf.at(1) == 'C' && buf.at(2) == 'K') {
          if (buf.at(5) == '4' && buf.at(6) == '@') {
            // Permission/Auth required
            flags.set(5);
            LOG_PRINT(level, LogLevel::WARNING, "WARNING: MPD requires auth!");
            return;
          } else {
            cleanup_close_tcp();
            flags.set(0);
            LOG_PRINT(level, LogLevel::ERROR,
                      "ERROR: Failed to \"status\" MPD (ACK)!");
            return;
          }
        } else {
          cleanup_close_tcp();
          flags.set(0);
          LOG_PRINT(level, LogLevel::ERROR,
                    "ERROR: Failed to \"status\" MPD (no OK)!");
          return;
        }
      } else {
        cleanup_close_tcp();
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to \"status\" MPD!");
        return;
      }

      std::this_thread::sleep_for(LOOP_SLEEP_TIME);
    } while (!successful_write_read);
  } else if (!flags.test(6)) {
    // Do "currentsong".
    bool successful_write_read = false;
    do {
      auto [status, buf] = write_read("currentsong\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (buf.at(buf.size() - 3) == 'O' && buf.at(buf.size() - 2) == 'K' &&
            buf.at(buf.size() - 1) == '\n') {
          // Success
          flags.set(6);
          successful_write_read = true;
          continue;
        } else if (buf.at(0) == 'A' && buf.at(1) == 'C' && buf.at(2) == 'K') {
          if (buf.at(5) == '4' && buf.at(6) == '@') {
            // Permission/Auth required
            flags.set(5);
            LOG_PRINT(level, LogLevel::WARNING, "WARNING: MPD requires auth!");
            return;
          } else {
            cleanup_close_tcp();
            flags.set(0);
            LOG_PRINT(level, LogLevel::ERROR,
                      "ERROR: Failed to \"currentsong\" MPD (ACK)!");
            return;
          }
        } else {
          cleanup_close_tcp();
          flags.set(0);
          LOG_PRINT(level, LogLevel::ERROR,
                    "ERROR: Failed to \"currentsong\" MPD (no OK)!");
          return;
        }
      } else {
        cleanup_close_tcp();
        flags.set(0);
        LOG_PRINT(level, LogLevel::ERROR,
                  "ERROR: Failed to \"currentsong\" MPD!");
        return;
      }

      std::this_thread::sleep_for(LOOP_SLEEP_TIME);
    } while (!successful_write_read);
  } else {
  }
}

std::tuple<MPDClient::StatusEnum, std::vector<char> > MPDClient::write_read(
    std::string to_send) {
  if (!is_ok() || tcp_socket < 0) {
    return {StatusEnum::SE_GENERIC_ERROR, {}};
  }

  LOG_PRINT(level, LogLevel::VERBOSE, "VERBOSE: sending: {}", to_send);

  bool did_write_this_iteration = false;
  if (!flags.test(4)) {
    ssize_t write_ret = write(tcp_socket, to_send.data(), to_send.size());
    if (write_ret == static_cast<ssize_t>(to_send.size())) {
      // Success.
      flags.set(4);
      did_write_this_iteration = true;
    } else if (write_ret < 0) {
      cleanup_close_tcp();
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to write \"{}\"! (errno {})", to_send, errno);
      return {};
    } else {
      cleanup_close_tcp();
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR, "ERROR: Failed to write \"{}\"!",
                to_send);
      return {StatusEnum::SE_GENERIC_ERROR, {}};
    }
  }

  std::vector<char> buf;
  buf.resize(READ_BUF_SIZE);

  ssize_t read_ret = read(tcp_socket, buf.data(), buf.size());
  if (read_ret > 0) {
    flags.reset(4);
    buf.resize(static_cast<size_t>(read_ret));
    LOG_PRINT(level, LogLevel::VERBOSE, "{:.{}s}", buf.data(), buf.size());
    return {StatusEnum::SE_SUCCESS, std::move(buf)};
  } else if (read_ret < 0) {
    if (errno == EAGAIN) {
      return {StatusEnum::SE_EAGAIN_ON_READ, {}};
    }
    cleanup_close_tcp();
    flags.set(0);
    if (did_write_this_iteration) {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to read after writing \"{}\"! (errno {})",
                to_send, errno);
    } else {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to read after writing! (errno {})", errno);
    }
    return {StatusEnum::SE_GENERIC_ERROR, {}};
  } else {
    cleanup_close_tcp();
    flags.set(0);
    if (did_write_this_iteration) {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Read EOF after writing \"{}\"! (errno {})", to_send,
                errno);
    } else {
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Read EOF after writing! (errno {})", errno);
    }
    return {StatusEnum::SE_GENERIC_ERROR, {}};
  }
}

void MPDClient::cleanup_close_tcp() {
  if (tcp_socket > 0) {
    close(tcp_socket);
    tcp_socket = -1;
  }
}
