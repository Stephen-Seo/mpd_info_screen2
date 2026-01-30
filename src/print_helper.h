#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_PRINT_HELPER_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_PRINT_HELPER_H_

#ifdef __cplusplus
#if __cplusplus >= 202302L

#include <cstdio>
#include <print>

namespace PrintHelper {
template <typename Arg, typename... Args>
void print(std::FILE *stream, std::format_string<Arg, Args...> fmt, Arg &&arg,
           Args &&...args) {
  std::print(stream, fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
}
template <typename... Args>
void print(std::FILE *stream, std::format_string<Args...> fmt, Args &&...args) {
  std::print(stream, fmt, std::forward<Args>(args)...);
}

template <typename Arg, typename... Args>
void print(std::format_string<Arg, Args...> fmt, Arg &&arg, Args &&...args) {
  std::print(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
}
template <typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args) {
  std::print(fmt, std::forward<Args>(args)...);
}

template <typename Arg, typename... Args>
void println(std::FILE *stream, std::format_string<Arg, Args...> fmt, Arg &&arg,
             Args &&...args) {
  std::println(stream, fmt, std::forward<Arg>(arg),
               std::forward<Args>(args)...);
}
template <typename... Args>
void println(std::FILE *stream, std::format_string<Args...> fmt,
             Args &&...args) {
  std::println(stream, fmt, std::forward<Args>(args)...);
}

template <typename Arg, typename... Args>
void println(std::format_string<Arg, Args...> fmt, Arg &&arg, Args &&...args) {
  std::println(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
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
template <typename Arg, typename... Args>
void print(std::FILE *stream, std::format_string<Arg, Args...> fmt, Arg &&arg,
           Args &&...args) {
  std::string s =
      std::format(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s", static_cast<int>(s.size()), s.c_str());
}
template <typename... Args>
void print(std::FILE *stream, std::format_string<Args...> fmt, Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s", static_cast<int>(s.size()), s.c_str());
}

template <typename Arg, typename... Args>
void print(std::format_string<Arg, Args...> fmt, Arg &&arg, Args &&...args) {
  std::string s =
      std::format(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
  std::printf("%.*s", static_cast<int>(s.size()), s.c_str());
}
template <typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::printf("%.*s", static_cast<int>(s.size()), s.c_str());
}

template <typename Arg, typename... Args>
void println(std::FILE *stream, std::format_string<Arg, Args...> fmt, Arg &&arg,
             Args &&...args) {
  std::string s =
      std::format(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s\n", static_cast<int>(s.size()), s.c_str());
}
template <typename... Args>
void println(std::FILE *stream, std::format_string<Args...> fmt,
             Args &&...args) {
  std::string s = std::format(fmt, std::forward<Args>(args)...);
  std::fprintf(stream, "%.*s\n", static_cast<int>(s.size()), s.c_str());
}

template <typename Arg, typename... Args>
void println(std::format_string<Arg, Args...> fmt, Arg &&arg, Args &&...args) {
  std::string s =
      std::format(fmt, std::forward<Arg>(arg), std::forward<Args>(args)...);
  std::printf("%.*s\n", static_cast<int>(s.size()), s.c_str());
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
