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
#include "mpd_client.h"

// third-party includes
#include <raylib.h>

MPDDisplay::MPDDisplay() : flags(), texture() {
  flags.set(0);
  flags.set(1);
}

MPDDisplay::~MPDDisplay() {
  if (texture) {
    UnloadTexture(*texture);
  }
}

MPDDisplay::MPDDisplay(MPDDisplay &&other)
    : flags(std::move(other.flags)), texture(std::move(other.texture)) {}

MPDDisplay &MPDDisplay::operator=(MPDDisplay &&other) {
  flags = std::move(other.flags);
  texture = std::move(other.texture);

  return *this;
}

void MPDDisplay::update(const MPDClient &cli) {
  if (!cli.is_ok()) {
    return;
  }

  if (cached_filename.empty() || cached_filename != cli.get_song_filename()) {
    flags.set(1);
    cached_filename = cli.get_song_filename();
  }

  if (!texture || flags.test(1)) {
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
        } else {
          texture.reset();
        }
      }
    }
  }

  if (flags.test(2) && !flags.test(1)) {
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
  }
}

void MPDDisplay::draw(const MPDClient &cli) {
  if (texture) {
    DrawTextureEx(*texture, {texture_x, texture_y}, 0.0F, texture_scale, WHITE);
  }
}

void MPDDisplay::request_reposition_texture() { flags.set(2); }
