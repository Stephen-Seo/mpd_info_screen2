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

// local includes
#include "args.h"
#include "constants.h"
#include "helpers.h"
#include "mpd_client.h"
#include "mpd_display.h"
#include "print_helper.h"
#include "signal_handler.h"
#include "version.h"

// Standard library includes
#include <atomic>
#include <chrono>
#include <fstream>

// third-party includes
#include <raylib.h>

constexpr Color CLEAR_BG_COLOR = {CLEAR_BG_COLOR_RGB, CLEAR_BG_COLOR_RGB,
                                  CLEAR_BG_COLOR_RGB, 255};

int main(int argc, char **argv) {
  Args args(argc, argv);

  if (args.is_error()) {
    if (args.get_flags().test(8)) {
      args.print_usage();
      return 0;
    } else if (args.get_flags().test(14)) {
      PrintHelper::println("Version {} of mpd_info_screen2.",
                           MPD_INFO_SCREEN_2_VERSION);
      return 0;
    }
    return 1;
  }

  MPDClient cli(args.get_host_ip_addr(), args.get_host_port(),
                args.get_log_level());

  if (!cli.is_ok()) {
    LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE,
              "VERBOSE: Client is NOT OK");
    return 2;
  } else {
    LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE, "VERBOSE: Client is OK");
  }

  std::unique_ptr<MPDDisplay> disp =
      std::make_unique<MPDDisplay>(args.get_flags(), args.get_log_level());

  int set_fps = TARGET_FPS;

  const auto do_auth = [&args, &cli, &disp, &set_fps]() {
    if (args.get_password_file().has_value()) {
      LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE,
                "VERBOSE: Attempting login...");
      std::string passwd;
      {
        std::ifstream ifs(args.get_password_file().value());
        if (!ifs.good()) {
          LOG_PRINT(args.get_log_level(), LogLevel::ERROR,
                    "ERROR: Failed to open password file \"{}\"!",
                    args.get_password_file().value());
          return;
        }

        while (!ifs.eof()) {
          auto c = ifs.get();
          if (ifs.good() && c != '\n' && c != '\r') {
            passwd.push_back(static_cast<char>(c));
          }
        }
      }
      if (!cli.attempt_auth(passwd)) {
        disp->set_failed_auth();
      }
    } else if (args.get_flags().test(6)) {
      auto fetched_pass = disp->fetch_prompted_pass();
      if (fetched_pass.has_value()) {
        if (!cli.attempt_auth(fetched_pass.value())) {
          disp->request_password_prompt();
        } else {
          if (set_fps != TARGET_FPS) {
            SetTargetFPS(TARGET_FPS);
            set_fps = TARGET_FPS;
          }
          disp->clear_cached_pass();
        }
        LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE,
                  "VERBOSE: Login attempted.");
      } else {
        if (set_fps != PPROMPT_FPS) {
          SetTargetFPS(PPROMPT_FPS);
          set_fps = PPROMPT_FPS;
        }
        disp->request_password_prompt();
      }
    } else {
      disp->request_password_prompt();
    }
  };

  InitWindow(800, 600, "mpd_info_screen2");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(TARGET_FPS);
  set_fps = TARGET_FPS;

  register_signals();

  auto update_time_point = std::chrono::steady_clock::now();
#ifndef NDEBUG
  auto print_info_time_point = std::chrono::steady_clock::now();
#endif

  while (!WindowShouldClose() &&
         !IS_SIGNAL_HANDLED.load(std::memory_order_relaxed)) {
    // update
    auto new_time_point = std::chrono::steady_clock::now();

    if (new_time_point - update_time_point > UPDATE_INFO_INTERVAL) {
      cli.request_data_update();
      update_time_point = new_time_point;
    }
#ifndef NDEBUG
    if (new_time_point - print_info_time_point > DEBUG_PRINT_INFO_INTERVAL) {
      LOG_PRINT(
          args.get_log_level(), LogLevel::DEBUG,
          "Title: {}\nArtist: {}\nAlbum: {}\nFilename: {}\nDuration: "
          "{}\nElapsed: {}\nAlbumArtSize: {}\nAlbumArtMimeType: {}",
          cli.get_song_title(), cli.get_song_artist(), cli.get_song_album(),
          cli.get_song_filename(), cli.get_song_duration(),
          std::get<double>(cli.get_elapsed_time()),
          cli.get_album_art().has_value() ? cli.get_album_art().value().size()
                                          : 0,
          cli.get_album_art_mime_type());
      print_info_time_point = new_time_point;
    }
#endif

    if (IsWindowResized()) {
      disp->request_reposition_texture();
    }

    cli.update();
    if (cli.needs_auth()) {
      do_auth();
    }
    disp->update(cli, args);

    // draw
    BeginDrawing();
    ClearBackground(CLEAR_BG_COLOR);
    disp->draw(cli, args);
    EndDrawing();
  }

  disp.reset();

  CloseWindow();

  return 0;
}
