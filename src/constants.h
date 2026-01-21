#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_CONSTANTS_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_CONSTANTS_H_

#include <chrono>
#include <iostream>
#include <print>

constexpr size_t READ_BUF_SIZE = 4096;
constexpr std::chrono::milliseconds LOOP_SLEEP_TIME =
    std::chrono::milliseconds(10);

#define LOG_PRINT(setting, level, msg, ...)       \
  if (log_level_can_log(setting, level)) {        \
    std::println(msg __VA_OPT__(, ) __VA_ARGS__); \
    std::cout.flush();                            \
  }

enum class LogLevel { SILENT, ERROR, WARNING, DEBUG, VERBOSE };

extern bool log_level_can_log(LogLevel setting, LogLevel level);

#endif
