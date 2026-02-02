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

#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_HELPERS_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_HELPERS_H_

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

extern constexpr bool helper_is_big_endian() {
  union {
    char c_arr[4];
    uint32_t u32;
  } u;

  u.u32 = 0x12345678;

  return u.c_arr[0] == 0x12;
}

template <typename T>
struct GenericCleanup {
  GenericCleanup(T *, std::function<void(T *)>);
  ~GenericCleanup();

  std::function<void(T *)> cleanupFn;
  T *ptr;
};

extern std::optional<uint32_t> helper_ipv4_str_to_value(std::string ipv4);

extern std::string helper_replace_in_string(const std::string &in,
                                            const std::string &target,
                                            const std::string &replacement);

extern std::string helper_unicode_font_fetch(
    const std::string &str_to_render,
    const std::unordered_set<std::string> &blacklist_strings);

extern std::string helper_str_to_lower(const std::string &s);

extern bool helper_str_is_ascii(const std::string &s);

extern uint32_t helper_uint32_byte_swap(uint32_t value);

//==============================================================================
// Template Definitions
//==============================================================================

template <typename T>
GenericCleanup<T>::GenericCleanup(T *ptr, std::function<void(T *)> cFn)
    : cleanupFn(cFn), ptr(ptr) {}

template <typename T>
GenericCleanup<T>::~GenericCleanup() {
  if (ptr) {
    cleanupFn(ptr);
  }
}

#endif
