#include "constants.h"

extern bool log_level_can_log(LogLevel setting, LogLevel level) {
  switch (setting) {
    case LogLevel::SILENT:
      return false;
    case LogLevel::ERROR:
      if (level == LogLevel::ERROR) {
        return true;
      } else {
        return false;
      }
    case LogLevel::WARNING:
      if (level == LogLevel::ERROR || level == LogLevel::WARNING) {
        return true;
      } else {
        return false;
      }
    case LogLevel::DEBUG:
      if (level == LogLevel::ERROR || level == LogLevel::WARNING ||
          level == LogLevel::DEBUG) {
        return true;
      } else {
        return false;
      }
    case LogLevel::VERBOSE:
      return true;
    default:
      return false;
  }
}
