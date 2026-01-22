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
  // if (helper_is_big_endian()) {
  //  htonl is a no-op on big endian systems. (Network byte order is Big-endian)
  //  Perhaps the way the 32-bit integer was constructed makes it ok for both
  //  big/little endian systems?
  //  result = htonl(result);
  //}

  return result;
}

extern std::string helper_replace_in_string(const std::string &in,
                                            const std::string &target,
                                            const std::string &replacement) {
  std::string ret = in;
  size_t idx = 0;
  while (idx != std::string::npos) {
    idx = ret.find(target, idx);
    if (idx != std::string::npos) {
      ret.replace(idx, target.size(), replacement);
      idx += replacement.size();
    }
  }
  return ret;
}
