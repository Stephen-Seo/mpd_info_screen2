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

#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_DISPLAY_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_DISPLAY_H_

// standard library includes
#include <bitset>
#include <memory>
#include <optional>
#include <string>

// local includes
#include "constants.h"

// forward declarations
class Args;
class MPDClient;
struct Texture;

class MPDDisplay {
 public:
  MPDDisplay(const std::bitset<64> &args_flags, LogLevel level);
  ~MPDDisplay();

  // no copy
  MPDDisplay(const MPDDisplay &) = delete;
  MPDDisplay &operator=(const MPDDisplay &) = delete;

  // allow move
  MPDDisplay(MPDDisplay &&);
  MPDDisplay &operator=(MPDDisplay &&);

  void update(const MPDClient &, const Args &);
  void draw(const MPDClient &, const Args &);

  void request_reposition_texture();

  void request_password_prompt();
  std::optional<std::string> fetch_prompted_pass();
  void set_failed_auth();
  void clear_cached_pass();

 private:
  LogLevel level;
  // 0 - need to refresh info
  // 1 - need to refetch texture
  // 2 - need to refresh texture positioning
  // 3 - prompt for password
  // 4 - has password
  // 5 - failed auth
  std::bitset<64> flags;
  std::unique_ptr<Texture> texture;
  std::string cached_filename;
  std::string cached_pass;
  std::string display_pass;
  std::string remaining_time;
  float texture_scale;
  float texture_x;
  float texture_y;
  float remaining_width;
  float remaining_height;
  int remaining_y_offset;

  void calculate_remaining_time_and_percent(const MPDClient &);
  void draw_remaining_time_and_percent(const MPDClient &, const Args &);
};

#endif
