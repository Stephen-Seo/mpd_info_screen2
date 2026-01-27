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
#include "constants.h"
#include "mpd_client.h"

// standard library includes
#include <chrono>
#include <format>

// third-party includes
#include <raylib.h>

MPDDisplay::MPDDisplay(const std::bitset<64> &args_flags, LogLevel level)
    : level(level), flags(), args_flags(args_flags), texture() {
  flags.set(0);
  flags.set(1);
}

MPDDisplay::~MPDDisplay() {
  if (texture) {
    UnloadTexture(*texture);
  }
}

MPDDisplay::MPDDisplay(MPDDisplay &&other)
    : level(other.level),
      flags(std::move(other.flags)),
      args_flags(std::move(other.args_flags)),
      texture(std::move(other.texture)) {}

MPDDisplay &MPDDisplay::operator=(MPDDisplay &&other) {
  level = other.level;
  flags = std::move(other.flags);
  args_flags = std::move(other.args_flags);
  texture = std::move(other.texture);

  return *this;
}

void MPDDisplay::update(const MPDClient &cli) {
  if (!cli.is_ok()) {
    return;
  }

  if (flags.test(3)) {
    // password prompt
    if (!args_flags.test(6)) {
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

  calculate_remaining_time_and_percent(cli);
}

void MPDDisplay::draw(const MPDClient &cli) {
  if (flags.test(5)) {
    DrawText("Failed authenticating to MPD!", 0, 0, 12, WHITE);
    return;
  } else if (flags.test(3)) {
    if (args_flags.test(6)) {
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

  if (!args_flags.test(9)) {
    draw_remaining_time_and_percent(cli);
  }
}

void MPDDisplay::request_reposition_texture() { flags.set(2); }

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

void MPDDisplay::calculate_remaining_time_and_percent(const MPDClient &cli) {
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

  if (remaining_i >= 60) {
    remaining_time = std::format("{}:{:02} {}%", remaining_i / 60,
                                 remaining_i % 60, percentage);
  } else {
    remaining_time =
        std::to_string(remaining_i) + " " + std::to_string(percentage) + "%";
  }

  // double width = GetScreenWidth();
  double height = GetScreenHeight();

  auto text_size = MeasureTextEx(GetFontDefault(), this->remaining_time.c_str(),
                                 TEXT_DEFAULT_SIZE, TEXT_DEFAULT_SIZE / 10.0F);

  remaining_y_offset = static_cast<int>(height - text_size.y);
}

void MPDDisplay::draw_remaining_time_and_percent(const MPDClient &cli) {
  DrawText(remaining_time.c_str(), 0, remaining_y_offset, TEXT_DEFAULT_SIZE,
           WHITE);
}
