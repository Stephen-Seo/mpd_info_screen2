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
#include "signal_handler.h"

// Standard library includes
#include <atomic>
#include <chrono>
#include <fstream>

// third-party includes
#include <raylib.h>

constexpr Color CLEAR_BG_COLOR = {50, 50, 50, 255};

int main(int argc, char **argv) {
  Args args(argc, argv);

  if (args.is_error()) {
    args.print_usage();
    return args.get_flags().test(8) ? 0 : 1;
  }

  MPDClient cli(args.get_host_ip_addr(), args.get_host_port(),
                LogLevel::VERBOSE);

  if (!cli.is_ok()) {
    LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE,
              "VERBOSE: Client is NOT OK");
    return 2;
  } else {
    LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE, "VERBOSE: Client is OK");
  }

  const auto do_auth = [&args, &cli]() {
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
      cli.attempt_auth(passwd);
      LOG_PRINT(args.get_log_level(), LogLevel::VERBOSE,
                "VERBOSE: Login attempted.");
    }
  };

  InitWindow(800, 600, "mpd_info_screen2");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(5);

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
      LOG_PRINT(args.get_log_level(), LogLevel::DEBUG,
                "Title: {}\nArtist: {}\nAlbum: {}\nFilename: {}\nDuration: "
                "{}\nElapsed: {}",
                cli.get_song_title(), cli.get_song_artist(),
                cli.get_song_album(), cli.get_song_filename(),
                cli.get_song_duration(), cli.get_elapsed_time());
      print_info_time_point = new_time_point;
    }
#endif

    cli.update();
    if (cli.needs_auth()) {
      do_auth();
    }

    // draw
    BeginDrawing();
    ClearBackground(CLEAR_BG_COLOR);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
