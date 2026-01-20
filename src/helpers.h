#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_HELPERS_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_HELPERS_H_

#include <cstdint>
#include <optional>
#include <string>

extern constexpr bool helper_is_big_endian() {
  union {
    char c_arr[4];
    uint32_t u32;
  } u;

  u.u32 = 0x12345678;

  return u.c_arr[0] == 0x12;
}

extern std::optional<uint32_t> helper_ipv4_str_to_value(std::string ipv4);

#endif
