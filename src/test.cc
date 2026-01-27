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

#include <atomic>
#include <print>

#include "helpers.h"
#include "mpd_client.h"

static std::atomic_uint64_t checked;
static std::atomic_uint64_t passed;

#define CHECK_TRUE(x)                                                    \
  do {                                                                   \
    ++checked;                                                           \
    if (x) {                                                             \
      ++passed;                                                          \
    } else {                                                             \
      std::println("ERROR: Line {}: \"{}\" is not true!", __LINE__, #x); \
    }                                                                    \
  } while (false);

#define CHECK_FALSE(x)                                                    \
  do {                                                                    \
    ++checked;                                                            \
    if (!(x)) {                                                           \
      ++passed;                                                           \
    } else {                                                              \
      std::println("ERROR: Line {}: \"{}\" is not false!", __LINE__, #x); \
    }                                                                     \
  } while (false);

int main(void) {
  // ipv4 str to value
  {
    std::optional<uint32_t> ipv4_opt;
    ipv4_opt = helper_ipv4_str_to_value("127.0.0.1");
    CHECK_TRUE(ipv4_opt.has_value());

    uint8_t *first_c = reinterpret_cast<uint8_t *>(&ipv4_opt.value());
    if (helper_is_big_endian()) {
      CHECK_TRUE(*first_c == 0x01);
    } else {
      CHECK_TRUE(*first_c == 0x7F);
    }
  }

  // MPDClient init
  {
    MPDClient cli("127.0.0.1.2", 2222, LogLevel::SILENT);
    CHECK_FALSE(cli.is_ok());
    cli = MPDClient("127.0.1", 3333, LogLevel::SILENT);
    CHECK_FALSE(cli.is_ok());
    cli = MPDClient("127.0.0.1", 4444, LogLevel::SILENT);
    CHECK_TRUE(cli.is_ok());
  }

  // helper_replace_in_string
  {
    std::string ret =
        helper_replace_in_string("one two three four five", "r", "arr");
    CHECK_TRUE(ret == "one two tharree fouarr five");
  }

  // helper unicode extract
  {
    std::string emoji("😀🐠");
    std::vector<uint8_t> ret = helper_unicode_extract_from_str(emoji);
    CHECK_TRUE(ret.at(0) == 0xF0);
    CHECK_TRUE(ret.at(1) == 0x9F);
    CHECK_TRUE(ret.at(2) == 0x90);
    CHECK_TRUE(ret.at(3) == 0xA0);
    ret = helper_unicode_extract_from_str(emoji);
    CHECK_TRUE(ret.at(0) == 0xF0);
    CHECK_TRUE(ret.at(1) == 0x9F);
    CHECK_TRUE(ret.at(2) == 0x98);
    CHECK_TRUE(ret.at(3) == 0x80);

    CHECK_TRUE(emoji.empty());
  }

  // helper get_font
  {
    std::string ascii("ok");
    std::string filename = helper_unicode_font_fetch(ascii);
    std::println("unicode_font_fetch: {}", filename);

    std::string emoji("😀🐠");
    filename = helper_unicode_font_fetch(emoji);
    std::println("unicode_font_fetch: {}", filename);
  }

  std::println("Checked: {}\nPassed: {}", checked.load(), passed.load());

  return checked.load() == passed.load() ? 0 : 1;
}
