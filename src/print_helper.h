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

#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_PRINT_HELPER_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_PRINT_HELPER_H_

#ifdef __cplusplus
#if __cplusplus >= 202302L

#include <cstdio>
#include <print>

namespace PrintHelper {
template <typename... Args>
void print(std::FILE *stream, std::format_string<Args...> fmt, Args &&...args) {
  std::print(stream, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args) {
  std::print(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void println(std::FILE *stream, std::format_string<Args...> fmt,
             Args &&...args) {
  std::println(stream, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void println(std::format_string<Args...> fmt, Args &&...args) {
  std::println(fmt, std::forward<Args>(args)...);
}

inline void println() { std::printf("\n"); }
}  // namespace PrintHelper

#else

#include <cstdio>
#include <format>

namespace PrintHelper {
template <typename... Args>
void print(std::FILE *stream, std::format_string<Args...> fmt, Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s", static_cast<int>(s.size()), s.c_str());
}

template <typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::printf("%.*s", static_cast<int>(s.size()), s.c_str());
}

template <typename... Args>
void println(std::FILE *stream, std::format_string<Args...> fmt,
             Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s\n", static_cast<int>(s.size()), s.c_str());
}

template <typename... Args>
void println(std::format_string<Args...> fmt, Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::printf("%.*s\n", static_cast<int>(s.size()), s.c_str());
}

inline void println() { std::printf("\n"); }
}  // namespace PrintHelper

#endif
#endif

#endif
