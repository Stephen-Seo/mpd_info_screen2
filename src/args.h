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

#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_ARGS_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_ARGS_H_

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_set>

// local includes
#include "constants.h"

class Args {
 public:
  Args(int argc, char **argv);

  static void print_usage();

  bool is_error() const;
  const std::bitset<64> &get_flags() const;
  const std::string &get_host_ip_addr() const;
  const std::optional<std::string> &get_password_file() const;
  double get_text_bg_opacity() const;
  LogLevel get_log_level() const;
  uint16_t get_host_port() const;
  const std::string &get_default_font_filename() const;
  const std::unordered_set<std::string> &get_font_blacklist_strings() const;

 private:
  // 0 - error parsing args
  // 1 - disable show title
  // 2 - disable show artist
  // 3 - disable show album
  // 4 - disable show filename
  // 5 - disable show percentage
  // 6 - prompt for password
  // 7 - don't scale fill the album art
  // 8 - -h or --help received
  // 9 - disable all text
  // 10 - only use default font
  // 11 - only use default font for ASCII
  // 12 - disable show remaining time
  // 13 - force remaining time to use default Raylib font
  // 14 - version specified
  std::bitset<64> flags;
  std::unordered_set<std::string> font_blacklist_strings;
  std::string host_ip_addr;
  std::string default_font_filename;
  std::optional<std::string> password_file;
  double text_bg_opacity;
  LogLevel level;
  uint16_t host_port;
};

#endif
