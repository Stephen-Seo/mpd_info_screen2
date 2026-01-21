#include "helpers.h"

// Unix includes
#include <arpa/inet.h>

std::optional<uint32_t> helper_ipv4_str_to_value(std::string ipv4) {
  uint32_t result;
  uint8_t *cptr = reinterpret_cast<uint8_t *>(&result);
  size_t cptr_idx = 0;

  int value = 0;

  for (char c : ipv4) {
    if (c >= '0' && c <= '9') {
      value = value * 10 + (c - '0');
    } else if (c == '.') {
      if (value > 0xFF || cptr_idx > 3) {
        return std::nullopt;
      }
      cptr[cptr_idx++] = static_cast<uint8_t>(value);
      value = 0;
    } else {
      return std::nullopt;
    }
  }

  if (value > 0) {
    if (value > 0xFF || cptr_idx > 3) {
      return std::nullopt;
    }
    cptr[cptr_idx++] = static_cast<uint8_t>(value);
  }

  if (cptr_idx != 4) {
    return std::nullopt;
  }

  // TODO Fix/Revise this
  if (helper_is_big_endian()) {
    result = htonl(result);
  }

  return result;
}
