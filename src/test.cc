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

  std::println("Checked: {}\nPassed: {}", checked.load(), passed.load());

  return checked.load() == passed.load() ? 0 : 1;
}
