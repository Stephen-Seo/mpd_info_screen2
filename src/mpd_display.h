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
#include <unordered_map>
#include <unordered_set>

// local includes
#include "constants.h"

// forward declarations
class Args;
class MPDClient;
struct Texture;
struct Font;

class FontWrapper {
 public:
  FontWrapper(std::string filename, std::string text);
  ~FontWrapper();

  // disallow copy
  FontWrapper(const FontWrapper &) = delete;
  FontWrapper &operator=(const FontWrapper &) = delete;

  // allow move
  FontWrapper(FontWrapper &&) = default;
  FontWrapper &operator=(FontWrapper &&) = default;

  const Font *get() const;

 private:
  std::unique_ptr<Font> font;
};

class MPDDisplay {
 private:
  enum TextType { TEXT_TITLE = 0, TEXT_ARTIST, TEXT_ALBUM, TEXT_FILENAME };

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
  // 0 - UNUSED
  // 1 - need to refetch texture
  // 2 - need to refresh texture positioning
  // 3 - prompt for password
  // 4 - has password
  // 5 - failed auth
  // 6 - attempted load default font
  // 7 - loaded/failed title font
  // 8 - loaded/failed artist font
  // 9 - loaded/failed album font
  // 10 - loaded/failed filename font
  std::bitset<64> flags;
  std::unique_ptr<Texture> texture;
  std::shared_ptr<Font> raylib_default_font;
  std::shared_ptr<Font> default_font;
  std::string cached_filename;
  std::string cached_pass;
  std::string display_pass;
  std::string remaining_time;
  std::unordered_map<int, FontWrapper> fonts;
  float texture_scale;
  float texture_x;
  float texture_y;
  float remaining_width;
  float remaining_height;
  float remaining_y_offset;
  float title_offset;
  float title_width;
  float title_height;
  float title_size;
  float artist_width;
  float artist_height;
  float artist_size;
  float artist_offset;
  float album_width;
  float album_height;
  float album_size;
  float album_offset;
  float filename_width;
  float filename_height;
  float filename_size;
  float filename_offset;

  void update_draw_texts(const MPDClient &, const Args &);
  void draw_draw_texts(const MPDClient &, const Args &);

  std::shared_ptr<Font> get_default_font();

  void load_draw_text_font(const std::string &text, TextType type);
};

#endif
