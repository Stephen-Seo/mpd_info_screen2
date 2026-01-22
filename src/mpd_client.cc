// ISC License
//
// Copyright (c) 2026 Stephen Seo
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include "mpd_client.h"

// Local includes
#include "constants.h"
#include "helpers.h"

// Standard library includes
#include <chrono>
#include <cstring>
#include <thread>
#include <vector>

// Unix includes
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

MPDClient::MPDClient(std::string host_ip, uint16_t host_port, LogLevel level)
    : flags(),
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
      level(std::move(other.level)),
      host_ip_value(std::move(other.host_ip_value)),
      host_port(other.host_port),
      tcp_socket(other.tcp_socket) {
  other.tcp_socket = -1;
}

MPDClient &MPDClient::operator=(MPDClient &&other) {
  this->flags = std::move(other.flags);
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

  uint8_t buf[READ_BUF_SIZE_SMALL];
  std::memset(buf, 0, READ_BUF_SIZE_SMALL);
  ssize_t read_ret = read(tcp_socket, buf, READ_BUF_SIZE_SMALL);
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

    int fcntl_ret = fcntl(tcp_socket, F_SETFD, O_NONBLOCK);
    if (fcntl_ret == -1) {
      cleanup_close_tcp();
      flags.set(0);
      LOG_PRINT(level, LogLevel::ERROR,
                "ERROR: Failed to set non-blocking on tcp socket! errno {}",
                errno);
      return;
    }

    flags.set(4);
    auto [status, str] = write_read("");
    if (flags.test(0)) {
      return;
    } else if (!str.empty()) {
      if (str.at(0) == 'O' && str.at(1) == 'K') {
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
      std::tie(status, str) = write_read("binarylimit 4000\n");
      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (str.at(0) == 'O' && str.at(1) == 'K') {
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
      auto [status, str] = write_read("ping\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (str.at(0) == 'O' && str.at(1) == 'K') {
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
      auto [status, str] = write_read("status\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (str.at(str.size() - 3) == 'O' && str.at(str.size() - 2) == 'K' &&
            str.at(str.size() - 1) == '\n') {
          // Success
          flags.set(3);
          successful_write_read = true;
          parse_for_song_info(str);
          continue;
        } else if (str.at(0) == 'A' && str.at(1) == 'C' && str.at(2) == 'K') {
          if (str.at(5) == '4' && str.at(6) == '@') {
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
      auto [status, str] = write_read("currentsong\n");

      if (flags.test(0) || (status != StatusEnum::SE_SUCCESS &&
                            status != StatusEnum::SE_EAGAIN_ON_READ)) {
        cleanup_close_tcp();
        flags.set(0);
        return;
      } else if (status == StatusEnum::SE_EAGAIN_ON_READ) {
        flags.set(4);
      } else if (status == StatusEnum::SE_SUCCESS) {
        if (str.at(str.size() - 3) == 'O' && str.at(str.size() - 2) == 'K' &&
            str.at(str.size() - 1) == '\n') {
          // Success
          flags.set(6);
          successful_write_read = true;
          parse_for_song_info(str);
          continue;
        } else if (str.at(0) == 'A' && str.at(1) == 'C' && str.at(2) == 'K') {
          if (str.at(5) == '4' && str.at(6) == '@') {
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

const std::string &MPDClient::get_song_title() const { return song_title; }
const std::string &MPDClient::get_song_artist() const { return song_artist; }
const std::string &MPDClient::get_song_album() const { return song_album; }
const std::string &MPDClient::get_song_filename() const {
  return song_filename;
}
double MPDClient::get_song_duration() const { return song_duration; }
double MPDClient::get_elapsed_time() const { return elapsed_time; }

void MPDClient::request_data_update() {
  flags.reset(3);
  flags.reset(6);
}

std::tuple<MPDClient::StatusEnum, std::string> MPDClient::write_read(
    std::string to_send) {
  if (!is_ok() || tcp_socket < 0) {
    return {StatusEnum::SE_GENERIC_ERROR, {}};
  }

  LOG_PRINT(level, LogLevel::VERBOSE, "VERBOSE: sending: {}", to_send);

  bool did_write_this_iteration = false;
  if (!flags.test(4)) {
    bool successful_write = false;
    do {
      ssize_t write_ret = write(tcp_socket, to_send.data(), to_send.size());
      if (write_ret == static_cast<ssize_t>(to_send.size())) {
        // Success.
        flags.set(4);
        did_write_this_iteration = true;
        successful_write = true;
        continue;
      } else if (write_ret < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          // Non-blocking IO, try again soon.
          std::this_thread::sleep_for(LOOP_SLEEP_TIME);
          continue;
        }
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
    } while (!successful_write);
  }

  std::string str;
  str.resize(READ_BUF_SIZE);

  const auto start_timestamp = std::chrono::steady_clock::now();
  bool successful_read = false;
  do {
    ssize_t read_ret = read(tcp_socket, str.data(), str.size());
    if (read_ret > 0) {
      flags.reset(4);
      successful_read = true;
      str.resize(static_cast<size_t>(read_ret));
      LOG_PRINT(level, LogLevel::VERBOSE, "{}", str);
      return {StatusEnum::SE_SUCCESS, std::move(str)};
    } else if (read_ret < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // Non-blocking IO, try again soon.
        const auto current_timestamp = std::chrono::steady_clock::now();
        if (current_timestamp - start_timestamp > MPD_CLI_READ_TIMEOUT) {
          LOG_PRINT(level, LogLevel::WARNING,
                    "WARNING: MPDCli read timed out!");
          return {StatusEnum::SE_READ_TIMED_OUT, {}};
        }
        std::this_thread::sleep_for(LOOP_SLEEP_TIME);
        continue;
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
  } while (!successful_read);

  return {StatusEnum::SE_GENERIC_ERROR, {}};
}

void MPDClient::cleanup_close_tcp() {
  if (tcp_socket > 0) {
    close(tcp_socket);
    tcp_socket = -1;
  }
}

void MPDClient::parse_for_song_info(const std::string &str) {
  size_t idx = 0;

  while (idx < str.size()) {
    if (str.size() - idx > 6 &&
        std::strncmp("Title: ", str.data() + idx, 6) == 0) {
      idx += 6;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      song_title = std::string(str.data() + idx, end_idx - idx);
      idx = end_idx + 1;
    } else if (str.size() - idx > 8 &&
               std::strncmp("Artist: ", str.data() + idx, 8) == 0) {
      idx += 8;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      song_artist = std::string(str.data() + idx, end_idx - idx);
      idx = end_idx + 1;
    } else if (str.size() - idx > 7 &&
               std::strncmp("Album: ", str.data() + idx, 7) == 0) {
      idx += 7;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      song_album = std::string(str.data() + idx, end_idx - idx);
      idx = end_idx + 1;
    } else if (str.size() - idx > 6 &&
               std::strncmp("file: ", str.data() + idx, 6) == 0) {
      idx += 6;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      song_filename = std::string(str.data() + idx, end_idx - idx);
      idx = end_idx + 1;
    } else if (str.size() - idx > 10 &&
               std::strncmp("duration: ", str.data() + idx, 10) == 0) {
      idx += 10;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      std::string song_duration_str =
          std::string(str.data() + idx, end_idx - idx);
      try {
        song_duration = std::stod(song_duration_str);
      } catch (const std::exception &e) {
        LOG_PRINT(level, LogLevel::WARNING,
                  "WARNING: Failed to parse song duration! {}",
                  song_duration_str);
      }
      idx = end_idx + 1;
    } else if (str.size() - idx > 9 &&
               std::strncmp("elapsed: ", str.data() + idx, 9) == 0) {
      idx += 9;
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      std::string song_elapsed_str =
          std::string(str.data() + idx, end_idx - idx);
      try {
        elapsed_time = std::stod(song_elapsed_str);
      } catch (const std::exception &e) {
        LOG_PRINT(level, LogLevel::WARNING,
                  "WARNING: Failed to parse song elapsed! {}",
                  song_elapsed_str);
      }
      idx = end_idx + 1;
    } else {
      size_t end_idx = str.find("\n", idx);
      if (end_idx == std::string::npos) {
        break;
      }
      idx = end_idx + 1;
    }
  }
}
