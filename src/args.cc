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

#include "args.h"

#include <cstdlib>
#include <cstring>
#include <print>

Args::Args(int argc, char **argv)
    : flags(),
      host_ip_addr(),
      password_file(),
      text_bg_opacity(0.745),
      level(LogLevel::ERROR),
      host_port(6600) {
  --argc;
  ++argv;
  while (argc > 0) {
    if (std::strncmp("--host=", argv[0], 7) == 0) {
      host_ip_addr = std::string(argv[0] + 7);
    } else if (std::strncmp("--port=", argv[0], 7) == 0) {
      unsigned long long p = std::strtoul(argv[0] + 7, nullptr, 10);
      if (p > 0xFFFF) {
        std::println(stderr, "ERROR: Port number {} too large!", p);
        flags.set(0);
        return;
      }
      host_port = static_cast<uint16_t>(p);
    } else if (std::strcmp("--disable-show-title", argv[0]) == 0) {
      flags.set(1);
    } else if (std::strcmp("--disable-show-artist", argv[0]) == 0) {
      flags.set(2);
    } else if (std::strcmp("--disable-show-album", argv[0]) == 0) {
      flags.set(3);
    } else if (std::strcmp("--disable-show-filename", argv[0]) == 0) {
      flags.set(4);
    } else if (std::strcmp("--disable-show-percentage", argv[0]) == 0) {
      flags.set(5);
    } else if (std::strcmp("--pprompt", argv[0]) == 0) {
      flags.set(6);
    } else if (std::strncmp("--pfile=", argv[0], 8) == 0) {
      password_file = std::string(argv[0] + 8);
    } else if (std::strcmp("--no-scale-fill", argv[0]) == 0) {
      flags.set(7);
    } else if (std::strncmp("--log-level=", argv[0], 12) == 0) {
      std::string level(argv[0] + 12);
      for (char &c : level) {
        if (c >= 'A' && c <= 'Z') {
          c += 32;
        }
      }
      if (level == "error") {
        this->level = LogLevel::ERROR;
      } else if (level == "warning") {
        this->level = LogLevel::WARNING;
      } else if (level == "debug") {
        this->level = LogLevel::DEBUG;
      } else if (level == "verbose") {
        this->level = LogLevel::VERBOSE;
      } else {
        std::println(stderr, "ERROR: Invalid log level \"{}\"!", argv[0] + 12);
        flags.set(0);
        return;
      }
    } else if (std::strncmp("--bg-opacity=", argv[0], 13) == 0) {
      text_bg_opacity = std::strtod(argv[0] + 13, nullptr);
      if (text_bg_opacity < 0.0 || text_bg_opacity > 1.0) {
        std::println(stderr,
                     "ERROR: --bg-opacity must be between 0.0 and 1.0!");
        flags.set(0);
        return;
      }
    } else if (std::strcmp("-h", argv[0]) == 0 ||
               std::strcmp("--help", argv[0]) == 0) {
      flags.set(0);
      flags.set(8);
      return;
    } else {
      std::println(stderr, "ERROR: Invalid arg: {}", argv[0]);
      flags.set(0);
      return;
    }

    --argc;
    ++argv;
  }

  if (host_ip_addr.empty()) {
    std::println(stderr, "ERROR: --host=<ip_addr> not specified!");
    flags.set(0);
    return;
  }
}

void Args::print_usage() {
  std::println("Usage:");
  std::println("  --host=<ip_addr> : ip address of mpd server");
  std::println("  --port=<port> : port of mpd server (default 6600)");
  std::println("  --disable-show-title : disable showing song title");
  std::println("  --disable-show-artist : disable showing song artist");
  std::println("  --disable-show-album : disable showing song album");
  std::println("  --disable-show-filename : disable showing song filename");
  std::println("  --disable-show-percentage : disable showing song percentage");
  std::println("  --pprompt : prompt for password on program start");
  std::println("  --pfile=<filename> : get password from specified file");
  std::println(
      "  --no-scale-fill : don't scale fill the album art to the window");
  std::println(
      "  --log-level=<level> : set the log level (ERROR, WARNING, DEBUG, "
      "VERBOSE)");
  std::println(
      "  --bg-opacity=<percentage> : set the text bg opacity by percentage "
      "(decimal point allowed)");
}

bool Args::is_error() const { return flags.test(0); }

const std::bitset<64> &Args::get_flags() const { return flags; }

const std::string &Args::get_host_ip_addr() const { return host_ip_addr; }

const std::optional<std::string> &Args::get_password_file() const {
  return password_file;
}

double Args::get_text_bg_opacity() const { return text_bg_opacity; }

LogLevel Args::get_log_level() const { return level; }

uint16_t Args::get_host_port() const { return host_port; }
