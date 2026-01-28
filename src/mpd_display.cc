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

#include "mpd_display.h"

// local includes
#include "args.h"
#include "constants.h"
#include "helpers.h"
#include "mpd_client.h"

// standard library includes
#include <chrono>
#include <format>
#include <vector>

// third-party includes
#include <raylib.h>

FontWrapper::FontWrapper(std::string filename, std::string text) {
  int codepoints_count = 0;
  int *codepoints = LoadCodepoints(text.c_str(), &codepoints_count);

  std::unordered_set<int> unique_codepoints;
  for (int idx = 0; idx < codepoints_count; ++idx) {
    unique_codepoints.insert(codepoints[idx]);
  }

  std::vector<int> codepoints_v;

  if (unique_codepoints.find('a') == unique_codepoints.end()) {
    // Force add 'a' and 'b' to codepoints to avoid unicode rendering bug.
    codepoints_v.push_back('a');
  }

  for (auto iter = unique_codepoints.begin(); iter != unique_codepoints.end();
       ++iter) {
    codepoints_v.push_back(*iter);
  }

  if (unique_codepoints.find('b') == unique_codepoints.end()) {
    // Force add 'a' and 'b' to codepoints to avoid unicode rendering bug.
    codepoints_v.push_back('b');
  }

#ifndef NDEBUG
  std::println("text: {}", text);
  std::println("codepoints:");
  for (size_t idx = 0; idx < codepoints_v.size(); ++idx) {
    std::print("{:02x} ", codepoints_v.at(idx));
  }
  std::println();
#endif

  Font f = LoadFontEx(
      filename.c_str(), static_cast<int>(MPDDisplay::scaled_font_size()),
      codepoints_v.data(), static_cast<int>(codepoints_v.size()));
  UnloadCodepoints(codepoints);
  if (f.baseSize != 0 && f.texture.id != GetFontDefault().texture.id) {
    SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
    font = std::make_unique<Font>(f);
  }
}

FontWrapper::~FontWrapper() {
  if (font) {
    UnloadFont(*font);
  }
}

const Font *FontWrapper::get() const {
  if (font) {
    return font.get();
  } else {
    return nullptr;
  }
}

MPDDisplay::MPDDisplay(const std::bitset<64> &args_flags, LogLevel level)
    : level(level), flags(), texture() {
  flags.set(1);
}

MPDDisplay::~MPDDisplay() {
  if (texture) {
    UnloadTexture(*texture);
  }

  if (default_font) {
    UnloadFont(*default_font);
  }
}

MPDDisplay::MPDDisplay(MPDDisplay &&other)
    : level(other.level),
      flags(std::move(other.flags)),
      texture(std::move(other.texture)) {}

MPDDisplay &MPDDisplay::operator=(MPDDisplay &&other) {
  level = other.level;
  flags = std::move(other.flags);
  texture = std::move(other.texture);

  return *this;
}

void MPDDisplay::update(const MPDClient &cli, const Args &args) {
  if (!cli.is_ok()) {
    return;
  }

  if (!raylib_default_font) {
    raylib_default_font = std::make_shared<Font>(GetFontDefault());
  }

  if (!flags.test(6) && !default_font &&
      !args.get_default_font_filename().empty()) {
    flags.set(6);
    Font f = LoadFontEx(args.get_default_font_filename().c_str(),
                        static_cast<int>(scaled_font_size()), nullptr, 0);
    if (f.baseSize != 0) {
      default_font = std::make_shared<Font>(f);
    }
  }

  if (flags.test(3)) {
    // password prompt
    if (!args.get_flags().test(6)) {
      return;
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !cached_pass.empty()) {
      if (cached_pass.back() & 0x80) {
        while ((cached_pass.back() & 0xC0) == 0x80) {
          cached_pass.pop_back();
        }
      }
      cached_pass.pop_back();
    } else if (IsKeyPressed(KEY_ENTER)) {
      flags.set(4);
      flags.reset(3);
      return;
    }

    int ret = 1;
    while (ret != 0) {
      ret = GetCharPressed();
      if (ret != 0) {
        if (ret <= 0x7F) {
          cached_pass.push_back(static_cast<char>(ret));
        } else {
          const char *utf8_parts = reinterpret_cast<char *>(&ret);
          for (size_t idx = 0; idx < 4; ++idx) {
            if (utf8_parts[idx] != 0) {
              cached_pass.push_back(utf8_parts[idx]);
            } else {
              break;
            }
          }
        }
      }
    }

    display_pass = std::string("password: ");
    for ([[maybe_unused]] char _unused : cached_pass) {
      display_pass.push_back('*');
    }

    return;
  }

  // Check if song changed, invalidate caches if so.
  if (cached_filename.empty() || cached_filename != cli.get_song_filename()) {
    flags.set(1);
    cached_filename = cli.get_song_filename();
    fonts.clear();

    flags.reset(7);
    flags.reset(8);
    flags.reset(9);
    flags.reset(10);
  }

  if (!texture || flags.test(1)) {
    // Load next album art image.
    const auto &cli_image = cli.get_album_art();
    if (cli_image.has_value()) {
      std::string ext;
      if (cli.get_album_art_mime_type() == "image/jpeg") {
        ext = ".jpg";
      } else if (cli.get_album_art_mime_type() == "image/png") {
        ext = ".png";
      } else if (cli.get_album_art_mime_type() == "image/gif") {
        ext = ".gif";
      }

      Image art_img = LoadImageFromMemory(
          ext.c_str(),
          reinterpret_cast<const unsigned char *>(cli_image.value().data()),
          static_cast<int>(cli_image.value().size()));
      if (art_img.data != nullptr) {
        if (texture) {
          UnloadTexture(*texture);
          texture.reset();
        }

        texture = std::make_unique<Texture>(LoadTextureFromImage(art_img));
        if (texture->width != 0 && texture->height != 0) {
          flags.set(2);
          flags.reset(1);
          UnloadImage(art_img);
          SetTextureFilter(*texture, TEXTURE_FILTER_BILINEAR);
        } else {
          texture.reset();
        }
      }
    }
  }

  if (flags.test(2) && !flags.test(1)) {
    // Calculate album art position.
    const int swidth = GetScreenWidth();
    const int sheight = GetScreenHeight();
    const float fswidth = static_cast<const float>(swidth);
    const float fsheight = static_cast<const float>(sheight);

    const float ftexture_w = static_cast<const float>(texture->width);
    const float ftexture_h = static_cast<const float>(texture->height);

    float xscale = fswidth / ftexture_w;
    float yscale = fsheight / ftexture_h;

    texture_scale = xscale > yscale ? yscale : xscale;

    texture_x = (fswidth - ftexture_w * texture_scale) / 2.0F;
    texture_y = (fsheight - ftexture_h * texture_scale) / 2.0F;

    flags.reset(2);
    flags.set(6);
  }

  if (!args.get_flags().test(9)) {
    update_draw_texts(cli, args);
  }
}

void MPDDisplay::draw(const MPDClient &cli, const Args &args) {
  if (flags.test(5)) {
    DrawText("Failed authenticating to MPD!", 0, 0, 12, WHITE);
    return;
  } else if (flags.test(3)) {
    if (args.get_flags().test(6)) {
      DrawText(display_pass.c_str(), 0, 0, 12, WHITE);
      return;
    } else {
      DrawText("Needs password, but --pprompt nor --pfile=<file> specified!", 0,
               0, 12, WHITE);
      return;
    }
  }

  if (texture) {
    DrawTextureEx(*texture, {texture_x, texture_y}, 0.0F, texture_scale, WHITE);
  }

  if (!args.get_flags().test(9) && !IsKeyDown(KEY_H)) {
    draw_draw_texts(cli, args);
  }
}

void MPDDisplay::request_reposition_texture() {
  flags.set(2);

  flags.reset(7);
  flags.reset(8);
  flags.reset(9);
  flags.reset(10);
  fonts.clear();
}

void MPDDisplay::request_password_prompt() {
  if (!flags.test(3)) {
    flags.set(3);
    flags.reset(4);
    cached_pass.clear();
  }
}

std::optional<std::string> MPDDisplay::fetch_prompted_pass() {
  if (flags.test(4) && !flags.test(3)) {
    return cached_pass;
  } else {
    return std::nullopt;
  }
}

void MPDDisplay::set_failed_auth() { flags.set(5); }

void MPDDisplay::clear_cached_pass() { cached_pass.clear(); }

float MPDDisplay::scaled_font_size() {
  return TEXT_DEFAULT_SIZE_F * static_cast<float>(GetScreenWidth()) / 800.0F;
}

void MPDDisplay::update_draw_texts(const MPDClient &cli, const Args &args) {
  auto now = std::chrono::steady_clock::now();
  double duration = cli.get_song_duration();
  int64_t duration_i = static_cast<int64_t>(duration);
  auto [elapsed, time_point] = cli.get_elapsed_time();

  auto time_diff = now - time_point;
  double time_diff_seconds = static_cast<double>(time_diff.count()) *
                             decltype(time_diff)::period::num /
                             decltype(time_diff)::period::den;

  double remaining = duration - elapsed - time_diff_seconds;

  int64_t remaining_i = static_cast<int64_t>(remaining);
  int64_t percentage =
      duration_i > 0 ? 100 * (duration_i - remaining_i) / duration_i : 0;

  if (!args.get_flags().test(12) && !args.get_flags().test(5)) {
    if (remaining_i >= 60) {
      remaining_time = std::format("{}:{:02} {}%", remaining_i / 60,
                                   remaining_i % 60, percentage);
    } else {
      remaining_time =
          std::to_string(remaining_i) + " " + std::to_string(percentage) + "%";
    }
  } else if (args.get_flags().test(5) && !args.get_flags().test(12)) {
    if (remaining_i >= 60) {
      remaining_time =
          std::format("{}:{:02}", remaining_i / 60, remaining_i % 60);
    } else {
      remaining_time = std::to_string(remaining_i);
    }
  } else if (args.get_flags().test(12) && !args.get_flags().test(5)) {
    remaining_time = std::to_string(percentage) + "%";
  } else {
    remaining_time.clear();
  }

  const int width = GetScreenWidth();
  float y_offset = static_cast<float>(GetScreenHeight());

  std::shared_ptr<Font> default_font = get_default_font();

  if (!args.get_flags().test(4) && !cli.get_song_filename().empty()) {
    Font font = *default_font;
    load_draw_text_font(cli.get_song_filename(), TEXT_FILENAME, args);
    auto fiter = fonts.find(TEXT_FILENAME);
    if (fiter != fonts.end()) {
      font = *fiter->second.get();
    }
    filename_size = scaled_font_size();
    Vector2 text_size;
    do {
      text_size = MeasureTextEx(font, cli.get_song_filename().c_str(),
                                static_cast<float>(filename_size),
                                static_cast<float>(filename_size) / 10.0F);
      if (text_size.x > static_cast<float>(width)) {
        --filename_size;
      }
      filename_width = text_size.x;
      filename_height = text_size.y;
    } while (text_size.x > static_cast<float>(width));
    y_offset -= filename_height;
    filename_offset = y_offset;
  }

  if (!args.get_flags().test(3) && !cli.get_song_album().empty()) {
    Font font = *default_font;
    load_draw_text_font(cli.get_song_album(), TEXT_ALBUM, args);
    auto fiter = fonts.find(TEXT_ALBUM);
    if (fiter != fonts.end()) {
      font = *fiter->second.get();
    }
    album_size = scaled_font_size();
    Vector2 text_size;
    do {
      text_size = MeasureTextEx(font, cli.get_song_album().c_str(),
                                static_cast<float>(album_size),
                                static_cast<float>(album_size) / 10.0F);
      if (text_size.x > static_cast<float>(width)) {
        --album_size;
      }
      album_width = text_size.x;
      album_height = text_size.y;
    } while (text_size.x > static_cast<float>(width));
    y_offset -= album_height;
    album_offset = y_offset;
  }

  if (!args.get_flags().test(2) && !cli.get_song_artist().empty()) {
    Font font = *default_font;
    load_draw_text_font(cli.get_song_artist(), TEXT_ARTIST, args);
    auto fiter = fonts.find(TEXT_ARTIST);
    if (fiter != fonts.end()) {
      font = *fiter->second.get();
    }
    artist_size = scaled_font_size();
    Vector2 text_size;
    do {
      text_size = MeasureTextEx(font, cli.get_song_artist().c_str(),
                                static_cast<float>(artist_size),
                                static_cast<float>(artist_size) / 10.0F);
      if (text_size.x > static_cast<float>(width)) {
        --artist_size;
      }
      artist_width = text_size.x;
      artist_height = text_size.y;
    } while (text_size.x > static_cast<float>(width));
    y_offset -= artist_height;
    artist_offset = y_offset;
  }

  if (!args.get_flags().test(1) && !cli.get_song_title().empty()) {
    Font font = *default_font;
    load_draw_text_font(cli.get_song_title(), TEXT_TITLE, args);
    auto fiter = fonts.find(TEXT_TITLE);
    if (fiter != fonts.end()) {
      font = *fiter->second.get();
    }
    title_size = scaled_font_size();
    Vector2 text_size;
    do {
      text_size = MeasureTextEx(font, cli.get_song_title().c_str(),
                                static_cast<float>(title_size),
                                static_cast<float>(title_size) / 10.0F);
      if (text_size.x > static_cast<float>(width)) {
        --title_size;
      }
      title_width = text_size.x;
      title_height = text_size.y;
    } while (text_size.x > static_cast<float>(width));
    y_offset -= title_height;
    title_offset = y_offset;
  }

  auto text_size =
      MeasureTextEx(*default_font, this->remaining_time.c_str(),
                    scaled_font_size(), scaled_font_size() / 10.0F);

  remaining_width = text_size.x;
  remaining_height = text_size.y;

  remaining_y_offset = y_offset - text_size.y;
}

void MPDDisplay::draw_draw_texts(const MPDClient &cli, const Args &args) {
  if (cli.get_play_state() == "stop") {
    DrawText("MPD is stopped", 0, 0, STATUS_TEXT_SIZE, WHITE);
  } else if (cli.get_play_state() == "pause") {
    DrawText("MPD is paused", 0, 0, STATUS_TEXT_SIZE, WHITE);
  } else {
    unsigned char opacity =
        static_cast<unsigned char>(args.get_text_bg_opacity() * 255);

    std::shared_ptr<Font> default_font = get_default_font();

    if (!remaining_time.empty()) {
      DrawRectangle(0, static_cast<int>(remaining_y_offset),
                    static_cast<int>(remaining_width),
                    static_cast<int>(remaining_height), {0, 0, 0, opacity});
      DrawTextEx(*default_font, remaining_time.c_str(), {0, remaining_y_offset},
                 scaled_font_size(), scaled_font_size() / 10.0F, WHITE);
    }
    if (!args.get_flags().test(1)) {
      Font font = *default_font;
      if (auto fiter = fonts.find(TEXT_TITLE); fiter != fonts.end()) {
        font = *fiter->second.get();
      }
      DrawRectangle(0, static_cast<int>(title_offset),
                    static_cast<int>(title_width),
                    static_cast<int>(title_height), {0, 0, 0, opacity});
      DrawTextEx(font, cli.get_song_title().c_str(), {0, title_offset},
                 title_size, title_size / 10.0F, WHITE);
      // TODO DEBUG
      // DrawTexture(font.texture, 0, 0, WHITE);
    }

    if (!args.get_flags().test(2)) {
      Font font = *default_font;
      if (auto fiter = fonts.find(TEXT_ARTIST); fiter != fonts.end()) {
        font = *fiter->second.get();
      }
      DrawRectangle(0, static_cast<int>(artist_offset),
                    static_cast<int>(artist_width),
                    static_cast<int>(artist_height), {0, 0, 0, opacity});
      DrawTextEx(font, cli.get_song_artist().c_str(), {0, artist_offset},
                 artist_size, artist_size / 10.0F, WHITE);
    }

    if (!args.get_flags().test(3)) {
      Font font = *default_font;
      if (auto fiter = fonts.find(TEXT_ALBUM); fiter != fonts.end()) {
        font = *fiter->second.get();
      }
      DrawRectangle(0, static_cast<int>(album_offset),
                    static_cast<int>(album_width),
                    static_cast<int>(album_height), {0, 0, 0, opacity});
      DrawTextEx(font, cli.get_song_album().c_str(), {0, album_offset},
                 album_size, album_size / 10.0F, WHITE);
    }

    if (!args.get_flags().test(4)) {
      Font font = *default_font;
      if (auto fiter = fonts.find(TEXT_FILENAME); fiter != fonts.end()) {
        font = *fiter->second.get();
      }
      DrawRectangle(0, static_cast<int>(filename_offset),
                    static_cast<int>(filename_width),
                    static_cast<int>(filename_height), {0, 0, 0, opacity});
      DrawTextEx(font, cli.get_song_filename().c_str(), {0, filename_offset},
                 filename_size, filename_size / 10.0F, WHITE);
    }
  }
}

std::shared_ptr<Font> MPDDisplay::get_default_font() {
  if (!default_font) {
    return raylib_default_font;
  }

  return default_font;
}

void MPDDisplay::load_draw_text_font(const std::string &text, TextType type,
                                     const Args &args) {
  if (text.empty()) {
    return;
  }
  switch (type) {
    case TEXT_TITLE:
      if (flags.test(7)) {
        return;
      }
      break;
    case TEXT_ARTIST:
      if (flags.test(8)) {
        return;
      }
      break;
    case TEXT_ALBUM:
      if (flags.test(9)) {
        return;
      }
      break;
    case TEXT_FILENAME:
      if (flags.test(10)) {
        return;
      }
      break;
  }

  if (fonts.find(type) == fonts.end()) {
    std::string filename;
    if (args.get_flags().test(10)) {
      filename = args.get_default_font_filename();
    } else if (helper_str_is_ascii(text) && args.get_flags().test(11)) {
      filename = args.get_default_font_filename();
    } else {
      filename = helper_unicode_font_fetch(text);
    }
    if (filename.empty()) {
      switch (type) {
        case TEXT_TITLE:
          flags.set(7);
          break;
        case TEXT_ARTIST:
          flags.set(8);
          break;
        case TEXT_ALBUM:
          flags.set(9);
          break;
        case TEXT_FILENAME:
          flags.set(10);
          break;
      }
      return;
    }
    FontWrapper font(filename, text);
    if (font.get() != nullptr) {
      fonts.insert(std::make_pair<int, FontWrapper>(type, std::move(font)));
    }
    switch (type) {
      case TEXT_TITLE:
        flags.set(7);
        break;
      case TEXT_ARTIST:
        flags.set(8);
        break;
      case TEXT_ALBUM:
        flags.set(9);
        break;
      case TEXT_FILENAME:
        flags.set(10);
        break;
    }
  }
}
