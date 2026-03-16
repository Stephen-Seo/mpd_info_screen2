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

// Standard library includes
#include <array>
#include <cstdlib>
#include <cstring>
#include <unordered_set>

// Local includes
#include "constants.h"
#include "print_helper.h"

// Third party includes
#include <raylib.h>

////////////////////////////////////////////////////////////////////////////////
// Internal functions
////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Color> INTERNAL_parse_str_to_Color(std::string str,
                                                   const char *opt_name) {
  std::array<double, 4> values;
  size_t values_idx = 0;
  size_t prev_idx = 0;
  size_t idx = 0;

  const auto parse_fn = [&]() -> bool {
    double *val = nullptr;
    try {
      val = &values.at(values_idx++);
    } catch (const std::exception &e) {
      PrintHelper::println(stderr, "ERROR: Too many values passed to \"{}\"!",
                           opt_name);
      return true;
    }

    if (!val) {
      PrintHelper::println(
          stderr, "ERROR: Invalid value passed to \"{}\" (nullptr)!", opt_name);
      return true;
    }

    try {
      *val = std::stod(str.substr(prev_idx, idx - prev_idx));
    } catch (const std::exception &e) {
      PrintHelper::println(stderr, "ERROR: Invalid value passed to \"{}\"!",
                           opt_name);
      return true;
    }
    prev_idx = idx + 1;

    return false;
  };

  while (idx < str.size()) {
    if (str.at(idx) == ',') {
      if (prev_idx < idx) {
        if (parse_fn()) {
          return {};
        }
      } else {
        PrintHelper::println(stderr,
                             "ERROR: Invalid value passed to "
                             "\"{}\" (invalid state)!",
                             opt_name);
        return {};
      }
    }

    ++idx;
  }

  if (prev_idx < idx) {
    if (parse_fn()) {
      return {};
    }
  }

  if (values_idx < values.size()) {
    PrintHelper::println(stderr, "ERROR: Not enough values passed to \"{}\"!",
                         opt_name);
    return {};
  }

  for (double val : values) {
    if (val < 0.0 || val > 1.0) {
      PrintHelper::println(stderr,
                           "ERROR: Value passed to \"{}\" is out of range "
                           "(must be between 0.0 and 1.0)!",
                           opt_name);
      return {};
    }
  }

  // Add 0.5 to round up when truncating from double to unsigned char.
  std::unique_ptr<Color> color = std::make_unique<Color>(
      Color{static_cast<uint8_t>(values.at(0) * 255.0 + 0.5),
            static_cast<uint8_t>(values.at(1) * 255.0 + 0.5),
            static_cast<uint8_t>(values.at(2) * 255.0 + 0.5),
            static_cast<uint8_t>(values.at(3) * 255.0 + 0.5)});

  return color;
}

////////////////////////////////////////////////////////////////////////////////
// END of Internal functions
////////////////////////////////////////////////////////////////////////////////

Args::Args(int argc, char **argv)
    : flags(),
      host_ip_addr(),
      default_font_filename(),
      password_file(),
      text_bg_opacity(0.745),
      font_scale_factor(1.0F),
      remaining_font_scale_factor(1.0F),
      level(LogLevel::ERROR),
      host_port(6600),
      bg_grayscale(CLEAR_BG_COLOR_RGB) {
  --argc;
  ++argv;
  while (argc > 0) {
    if (std::strncmp("--host=", argv[0], 7) == 0) {
      host_ip_addr = std::string(argv[0] + 7);
    } else if (std::strncmp("--port=", argv[0], 7) == 0) {
      unsigned long long p = std::strtoul(argv[0] + 7, nullptr, 10);
      if (p > 0xFFFF) {
        PrintHelper::println(stderr, "ERROR: Port number {} too large!", p);
        flags.set(0);
        return;
      }
      host_port = static_cast<uint16_t>(p);
    } else if (std::strcmp("--disable-all-text", argv[0]) == 0) {
      flags.set(9);
    } else if (std::strcmp("--disable-show-title", argv[0]) == 0) {
      flags.set(1);
    } else if (std::strcmp("--disable-show-artist", argv[0]) == 0) {
      flags.set(2);
    } else if (std::strcmp("--disable-show-album", argv[0]) == 0) {
      flags.set(3);
    } else if (std::strcmp("--disable-show-filename", argv[0]) == 0) {
      flags.set(4);
    } else if (std::strcmp("--disable-show-remaining", argv[0]) == 0) {
      flags.set(12);
    } else if (std::strcmp("--disable-show-percentage", argv[0]) == 0) {
      flags.set(5);
    } else if (std::strcmp("--align-text-right", argv[0]) == 0) {
      flags.set(15);
    } else if (std::strcmp("--pprompt", argv[0]) == 0) {
      flags.set(6);
    } else if (std::strncmp("--pfile=", argv[0], 8) == 0) {
      password_file = std::string(argv[0] + 8);
    } else if (std::strcmp("--no-scale-fill", argv[0]) == 0) {
      flags.set(7);
    } else if (std::strcmp("--align-album-art-left", argv[0]) == 0) {
      if (flags.test(17)) {
        PrintHelper::println(stderr,
                             "ERROR: --align-album-art-left and "
                             "--align-album-art-right are mutally exclusive!");
        flags.set(0);
        return;
      }
      flags.set(16);
    } else if (std::strcmp("--align-album-art-right", argv[0]) == 0) {
      if (flags.test(16)) {
        PrintHelper::println(stderr,
                             "ERROR: --align-album-art-left and "
                             "--align-album-art-right are mutally exclusive!");
        flags.set(0);
        return;
      }
      flags.set(17);
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
        PrintHelper::println(stderr, "ERROR: Invalid log level \"{}\"!",
                             argv[0] + 12);
        flags.set(0);
        return;
      }
    } else if (std::strncmp("--bg-opacity=", argv[0], 13) == 0) {
      text_bg_opacity = std::strtod(argv[0] + 13, nullptr);
      if (text_bg_opacity < 0.0 || text_bg_opacity > 1.0) {
        PrintHelper::println(
            stderr, "ERROR: --bg-opacity must be between 0.0 and 1.0!");
        flags.set(0);
        return;
      }
    } else if (std::strncmp("--default-font-filename=", argv[0], 24) == 0) {
      default_font_filename = std::string(argv[0] + 24);
    } else if (std::strcmp("--force-default-font", argv[0]) == 0) {
      flags.set(10);
    } else if (std::strcmp("--force-default-font-ascii", argv[0]) == 0) {
      flags.set(11);
    } else if (std::strncmp("--blacklist-font-str=", argv[0], 21) == 0) {
      std::string string(argv[0] + 21);
      if (!string.empty()) {
        font_blacklist_strings.insert(string);
      }
    } else if (std::strncmp("--whitelist-font-str=", argv[0], 21) == 0) {
      std::string string(argv[0] + 21);
      if (!string.empty()) {
        font_whitelist_strings.insert(string);
      }
    } else if (std::strcmp("--remaining-force-default-raylib-font", argv[0]) ==
               0) {
      flags.set(13);
    } else if (std::strncmp("--font-scale-factor=", argv[0], 20) == 0) {
      font_scale_factor = std::strtof(argv[0] + 20, nullptr);
      if (font_scale_factor <= 0.0 ||
          font_scale_factor >= FONT_SCALE_FACTOR_MAX) {
        PrintHelper::println(
            stderr, "ERROR: --font-scale-factor must be between 0.0 and {}!",
            FONT_SCALE_FACTOR_MAX);
        flags.set(0);
        return;
      }
    } else if (std::strncmp("--remaining-font-scale-factor=", argv[0], 30) ==
               0) {
      remaining_font_scale_factor = std::strtof(argv[0] + 30, nullptr);
      if (remaining_font_scale_factor <= 0.0 ||
          remaining_font_scale_factor >= FONT_SCALE_FACTOR_MAX) {
        PrintHelper::println(
            stderr,
            "ERROR: --remaining-font-scale-factor must be between 0.0 and {}!",
            FONT_SCALE_FACTOR_MAX);
        flags.set(0);
        return;
      }
      flags.set(18);
    } else if (std::strcmp("--h-toggles-text", argv[0]) == 0) {
      flags.set(19);
    } else if (std::strncmp("--background-color=", argv[0], 19) == 0) {
      std::string string(argv[0] + 19);
      double value;
      try {
        value = std::stod(string);
      } catch (const std::exception &e) {
        PrintHelper::println(
            stderr,
            "ERROR: Invalid value passed to --background-color=<value>!");
        flags.set(0);
        return;
      }
      if (value < 0.0) {
        PrintHelper::println(
            stderr,
            "ERROR: Value passed to --background-color=<value> is too low "
            "(must be at least 0.0)!");
        flags.set(0);
        return;
      } else if (value > 1.0) {
        PrintHelper::println(
            stderr,
            "ERROR: Value passed to --background-color=<value> is too high "
            "(must be at most 1.0)!");
        flags.set(0);
        return;
      }
      // Add 0.5 to round to the nearest integer, since it truncates when
      // converting.
      bg_grayscale = static_cast<uint8_t>(value * 255.0 + 0.5);
    } else if (std::strncmp("--text-fg-color=", argv[0], 16) == 0) {
      std::string str(argv[0] + 16);
      std::unique_ptr<Color> color =
          INTERNAL_parse_str_to_Color(str, "--text-fg-color");
      if (!color) {
        flags.set(0);
        return;
      }
      this->text_fg_color = std::move(color);
    } else if (std::strncmp("--text-bg-color=", argv[0], 16) == 0) {
      std::string str(argv[0] + 16);
      std::unique_ptr<Color> color =
          INTERNAL_parse_str_to_Color(str, "--text-bg-color");
      if (!color) {
        flags.set(0);
        return;
      }
      this->text_bg_color = std::move(color);
    } else if (std::strcmp("--version", argv[0]) == 0) {
      flags.set(0);
      flags.set(14);
      return;
    } else if (std::strcmp("-h", argv[0]) == 0 ||
               std::strcmp("--help", argv[0]) == 0) {
      flags.set(0);
      flags.set(8);
      return;
    } else {
      PrintHelper::println(stderr, "ERROR: Invalid arg: {}", argv[0]);
      flags.set(0);
      return;
    }

    --argc;
    ++argv;
  }

  if (host_ip_addr.empty()) {
    PrintHelper::println(stderr, "ERROR: --host=<ip_addr> not specified!");
    flags.set(0);
    return;
  }
}

void Args::print_usage() {
  PrintHelper::println("Usage:");
  PrintHelper::println("  -h | --help : show this usage text");
  PrintHelper::println("  --version : show the version of this program");
  PrintHelper::println("  --host=<ip_addr> : ip address of mpd server");
  PrintHelper::println("  --port=<port> : port of mpd server (default 6600)");
  PrintHelper::println("  --disable-all-text : disables showing all text");
  PrintHelper::println("  --disable-show-title : disable showing song title");
  PrintHelper::println("  --disable-show-artist : disable showing song artist");
  PrintHelper::println("  --disable-show-album : disable showing song album");
  PrintHelper::println(
      "  --disable-show-filename : disable showing song filename");
  PrintHelper::println(
      "  --disable-show-remaining : disables showing remaining time");
  PrintHelper::println(
      "  --disable-show-percentage : disable showing song percentage");
  PrintHelper::println("  --align-text-right : Aligns the text to the right");
  PrintHelper::println("  --pprompt : prompt for password on program start");
  PrintHelper::println(
      "  --pfile=<filename> : get password from specified file");
  PrintHelper::println(
      "  --no-scale-fill : don't scale fill the album art to the window");
  PrintHelper::println(
      "  --align-album-art-left : align the album art to the left");
  PrintHelper::println(
      "  --align-album-art-right: align the album art to the right");
  PrintHelper::println(
      "  --log-level=<level> : set the log level (ERROR, WARNING, DEBUG, "
      "VERBOSE)");
  PrintHelper::println(
      "  --bg-opacity=<percentage> : set the text bg opacity by percentage "
      "(decimal point allowed)");
  PrintHelper::println(
      "  --default-font-filename=<font_filename> : set the default font");
  PrintHelper::println(
      "  --force-default-font : Only use the default font (mutually exclusive "
      "with next option)");
  PrintHelper::println(
      "  --force-default-font-ascii : Only use the default font for ascii text "
      "(mutually exclusive with previous option)");
  PrintHelper::println(
      "  --blacklist-font-str=<string> : blacklist fonts that have <string> in "
      "its filename (use this option multiple times to add more strings to "
      "check)");
  PrintHelper::println(
      "  --whitelist-font-str=<string> : whitelist fonts that have <string> in "
      "its filename (use this option multiple times to add more strings to "
      "check; if a font matches ANY strings in the whitelist, it is allowed)");
  PrintHelper::println(
      "  --remaining-force-default-raylib-font : force the remaining time text "
      "to always use Raylib's default font");
  PrintHelper::println(
      "  --font-scale-factor=<factor> : Sets the factor to scale the font size "
      "with (default 1.0)");
  PrintHelper::println(
      "  --remaining-font-scale-factor=<factor> : Sets the factor to scale the "
      "remaining (remaining time and elapsed percentage) text's font size with "
      "(default 1.0)");
  PrintHelper::println(
      "  --h-toggles-text : Make the \"H\" key toggle displaying text instead "
      "of only hiding while pressed");
  PrintHelper::println(
      "  --background-color=<value> : Sets the grayscale color of the "
      "background (between 0.0 and 1.0; black and white)");
  PrintHelper::println(
      "  --text-fg-color=<RED>,<BLUE>,<GREEN>,<ALPHA> : Sets the fg-color of "
      "the main displayed text. Expects values between 0.0 and 1.0 per item");
  PrintHelper::println(
      "  --text-bg-color=<RED>,<BLUE>,<GREEN>,<ALPHA> : Sets the bg-color of "
      "the main displayed text. Expects values between 0.0 and 1.0 per item");
}

bool Args::is_error() const { return flags.test(0); }

const std::bitset<64> &Args::get_flags() const { return flags; }

const std::string &Args::get_host_ip_addr() const { return host_ip_addr; }

const std::string &Args::get_default_font_filename() const {
  return default_font_filename;
}

const std::unordered_set<std::string> &Args::get_font_blacklist_strings()
    const {
  return font_blacklist_strings;
}

const std::unordered_set<std::string> &Args::get_font_whitelist_strings()
    const {
  return font_whitelist_strings;
}

const std::optional<std::string> &Args::get_password_file() const {
  return password_file;
}

double Args::get_text_bg_opacity() const { return text_bg_opacity; }

float Args::get_font_scale_factor() const { return font_scale_factor; }

float Args::get_remaining_font_scale_factor() const {
  return remaining_font_scale_factor;
}

LogLevel Args::get_log_level() const { return level; }

uint16_t Args::get_host_port() const { return host_port; }

uint8_t Args::get_bg_grayscale() const { return bg_grayscale; }

const std::unique_ptr<Color> &Args::get_text_fg_color() const {
  return text_fg_color;
}

const std::unique_ptr<Color> &Args::get_text_bg_color() const {
  return text_bg_color;
}
