#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_ARGS_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_ARGS_H_

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>

enum class LogLevel { ERROR, WARNING, DEBUG, VERBOSE };

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
  std::bitset<64> flags;
  std::string host_ip_addr;
  std::optional<std::string> password_file;
  double text_bg_opacity;
  LogLevel level;
  uint16_t host_port;
};

#endif
