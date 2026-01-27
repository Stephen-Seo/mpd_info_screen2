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

// Standard library includes
#include <cstring>

// Unix includes
#include <arpa/inet.h>

// Third party includes
#include <fontconfig/fontconfig.h>

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

extern std::string helper_unicode_font_fetch(const std::string &str_to_render) {
  FcConfig *config = FcInitLoadConfigAndFonts();
  GenericCleanup<FcConfig *> config_cleanup(&config, [](FcConfig **config) {
    if (*config) {
      FcConfigDestroy(*config);
      *config = nullptr;
    }
  });

  if (!config) {
    return {};
  }

  FcCharSet *fcCharSet = FcCharSetCreate();
  GenericCleanup<FcCharSet *> charset_cleanup(&fcCharSet, [](FcCharSet **set) {
    if (*set) {
      FcCharSetDestroy(*set);
      *set = nullptr;
    }
  });

  if (!fcCharSet) {
    return {};
  }

  std::string str_copy = str_to_render;
  while (!str_copy.empty()) {
    std::vector<uint8_t> unicode = helper_unicode_extract_from_str(str_copy);
    if (unicode.empty() || unicode.size() > 4) {
      continue;
    }

    uint32_t codepoint = 0;
    uint8_t *ptr = reinterpret_cast<uint8_t *>(&codepoint);
    for (size_t idx = 0; idx < unicode.size(); ++idx) {
      ptr[idx] = unicode[idx];
    }
    FcCharSetAddChar(fcCharSet, codepoint);
  }

  FcPattern *fcPattern = FcPatternCreate();
  GenericCleanup<FcPattern *> pattern_cleanup(&fcPattern, [](FcPattern **pat) {
    if (*pat) {
      FcPatternDestroy(*pat);
      *pat = nullptr;
    }
  });

  if (!fcPattern) {
    return {};
  }

  FcValue value;
  value.type = FcTypeCharSet;
  value.u.c = fcCharSet;

  FcConfigSetDefaultSubstitute(config, fcPattern);
  if (FcPatternAdd(fcPattern, FC_CHARSET, value, FcTrue) == FcFalse) {
    return {};
  }

  if (FcConfigSubstitute(config, fcPattern, FcMatchPattern) != FcTrue) {
    return {};
  }

  FcResult res;
  FcPattern *retPattern = FcFontMatch(config, fcPattern, &res);

  GenericCleanup<FcPattern *> ret_pattern_cleanup(&retPattern,
                                                  [](FcPattern **p) {
                                                    if (*p) {
                                                      FcPatternDestroy(*p);
                                                      *p = nullptr;
                                                    }
                                                  });

  if (res != FcResultMatch || !retPattern) {
    return {};
  }

  FcObjectSet *fcObjectSet = FcObjectSetCreate();

  GenericCleanup<FcObjectSet *> objectset_cleanup(&fcObjectSet,
                                                  [](FcObjectSet **set) {
                                                    if (*set) {
                                                      FcObjectSetDestroy(*set);
                                                      *set = nullptr;
                                                    }
                                                  });

  if (!fcObjectSet) {
    return {};
  }

  if (FcObjectSetAdd(fcObjectSet, FC_FILE) == FcFalse) {
    return {};
  }

  FcPattern *filteredPattern = FcPatternFilter(retPattern, fcObjectSet);

  GenericCleanup<FcPattern *> filteredpattern_cleanup(
      &filteredPattern, [](FcPattern **pat) {
        if (*pat) {
          FcPatternDestroy(*pat);
          *pat = nullptr;
        }
      });

  if (!filteredPattern) {
    return {};
  }

  FcValue ret_patget;
  std::memset(&ret_patget, 0, sizeof(FcValue));

  if (FcPatternGet(filteredPattern, FC_FILE, 0, &ret_patget) != FcResultMatch) {
    return {{}, {}};
  }

  return std::string(reinterpret_cast<const char *>(ret_patget.u.s));
}

std::vector<uint8_t> helper_unicode_extract_from_str(std::string &str) {
  std::vector<uint8_t> v;

  uint8_t back = static_cast<uint8_t>(str.back());
  if (back < 0x7F) {
    str.pop_back();
    v.push_back(back);
    return v;
  }

  size_t count = 0;
  while (!str.empty() && count < 4) {
    uint8_t back = static_cast<uint8_t>(str.back());
    str.pop_back();
    v.push_back(back);
    if ((back & 0xC0) != 0x80) {
      break;
    }
  }

  std::vector<uint8_t> rev;
  for (auto iter = v.crbegin(); iter != v.crend(); ++iter) {
    rev.push_back(*iter);
  }

  return rev;
}
