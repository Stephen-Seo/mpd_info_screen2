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
#include <raylib.h>

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

  // TODO Verify this works
  if (helper_is_big_endian()) {
    result = helper_uint32_byte_swap(result);
  }

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

extern std::string helper_unicode_font_fetch(
    const std::string &str_to_render,
    const std::unordered_set<std::string> &blacklist_strings) {
  if (FcInit() != FcTrue) {
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

  int codepoint_len = 0;
  int *codepoints = LoadCodepoints(str_to_render.c_str(), &codepoint_len);

  GenericCleanup<int *> codepoints_cleanup(&codepoints, [](int **codepoints) {
    if (*codepoints) {
      UnloadCodepoints(*codepoints);
      *codepoints = nullptr;
    }
  });

  for (int idx = 0; idx < codepoint_len; ++idx) {
    FcCharSetAddChar(fcCharSet, static_cast<FcChar32>(codepoints[idx]));
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

  if (FcPatternAdd(fcPattern, FC_CHARSET, value, FcTrue) == FcFalse) {
    return {};
  }

  FcObjectSet *filename_objset = FcObjectSetBuild(FC_FILE, nullptr);
  GenericCleanup<FcObjectSet *> filename_objs_cleanup(
      &filename_objset, [](FcObjectSet **set) {
        if (*set) {
          FcObjectSetDestroy(*set);
          *set = nullptr;
        }
      });

  FcFontSet *fset = FcFontList(nullptr, fcPattern, filename_objset);
  GenericCleanup<FcFontSet *> fset_cleanup(&fset, [](FcFontSet **set) {
    if (*set) {
      FcFontSetDestroy(*set);
      *set = nullptr;
    }
  });

  if (!fset) {
    return {};
  } else if (fset->nfont == 0) {
    return {};
  }

  for (int idx = 0; idx < fset->nfont; ++idx) {
    FcPattern *pat = fset->fonts[idx];

    FcChar8 *file;

    if (FcPatternGetString(pat, FC_FILE, 0, &file) == FcResultMatch) {
      std::string inner_filename(reinterpret_cast<const char *>(file));
      if (auto idx = inner_filename.find(".ttf");
          idx != std::string::npos && idx + 4 == inner_filename.size()) {
        bool blacklisted = false;
        for (const std::string &blacklist_str : blacklist_strings) {
          if (auto idx = inner_filename.find(blacklist_str);
              idx != std::string::npos) {
            // Blacklisted string in filename, don't use this font.
            blacklisted = true;
            break;
          }
        }
        if (blacklisted) {
          continue;
        }
        return inner_filename;
      }
    }
  }

  return {};
}

std::string helper_str_to_lower(const std::string &s) {
  std::string ret;

  for (char c : s) {
    if (c >= 0x41 && c <= 0x5A) {
      c += 0x20;
    }
    ret.push_back(c);
  }

  return ret;
}

bool helper_str_is_ascii(const std::string &s) {
  for (char c : s) {
    if (c & 0x80) {
      return false;
    }
  }
  return true;
}

uint32_t helper_uint32_byte_swap(uint32_t value) {
  uint32_t ret;
  uint8_t *ret_ptr = reinterpret_cast<uint8_t *>(&ret);
  const uint8_t *val_ptr = reinterpret_cast<const uint8_t *>(&value);

  for (int idx = 0; idx < 4; ++idx) {
    ret_ptr[3 - idx] = val_ptr[idx];
  }

  return ret;
}
