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
#include "host_prompt.h"
#include "mpd_client.h"
#include "mpd_display.h"
#include "print_helper.h"
#include "signal_handler.h"
#include "version.h"

// Standard library includes
#include <atomic>
#include <chrono>
#include <fstream>
#include <optional>

// third-party includes
#include <raylib.h>

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

  const Color CLEAR_BG_COLOR{args.get_bg_grayscale(), args.get_bg_grayscale(),
                             args.get_bg_grayscale(), 255};

  InitWindow(800, 600, "mpd_info_screen2");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(PPROMPT_FPS);

  // First draw to clear screen.
  BeginDrawing();
  ClearBackground(CLEAR_BG_COLOR);
  EndDrawing();

  {
    HostPrompt host_prompt{};
    while (args.get_host_ip_addr().empty() &&
           args.get_host_unix_socket().empty()) {
      // Prompt for host addr.
      if (host_prompt.update()) {
        if (!host_prompt.get_addr().empty()) {
          args.set_host_ip_addr(host_prompt.get_addr());
        } else if (!host_prompt.get_socket().empty()) {
          args.set_host_socket(host_prompt.get_socket());
        }
      }
      BeginDrawing();
      ClearBackground(CLEAR_BG_COLOR);
      host_prompt.draw();
      EndDrawing();

      if (WindowShouldClose()) {
        CloseWindow();
        return 0;
      }
    }
  }

  BeginDrawing();
  ClearBackground(CLEAR_BG_COLOR);
  EndDrawing();

  MPDClient cli(args.is_using_unix_socket() ? args.get_host_unix_socket()
                                            : args.get_host_ip_addr(),
                args.get_host_port(), args.get_log_level(),
                args.is_using_unix_socket());

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
    } else {
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
    }
  };

  SetTargetFPS(TARGET_FPS);
  set_fps = TARGET_FPS;

  register_signals();

  auto update_time_point = std::chrono::steady_clock::now();
#ifndef NDEBUG
  auto print_info_time_point = std::chrono::steady_clock::now();
#endif
  std::optional<decltype(update_time_point)> reconnect_time_point =
      std::nullopt;

  std::optional<std::string> message;

  int reconnect_attempts = 0;

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
      disp->request_reposition_texture(args);
    }

    if (!cli.is_ok() &&
        (cli.ping_success() || reconnect_attempts < MAX_RECONNECT_ATTEMPTS)) {
      if (reconnect_time_point.has_value()) {
        if (new_time_point - reconnect_time_point.value() >
            RECONNECT_INTERVAL) {
          reconnect_time_point = std::nullopt;
          cli = MPDClient(args.is_using_unix_socket()
                              ? args.get_host_unix_socket()
                              : args.get_host_ip_addr(),
                          args.get_host_port(), args.get_log_level(),
                          args.is_using_unix_socket());
          disp = std::make_unique<MPDDisplay>(args.get_flags(),
                                              args.get_log_level());
        }
      } else {
        reconnect_time_point = new_time_point;
        if (cli.ping_success()) {
          reconnect_attempts = 1;
          message.reset();
        } else {
          ++reconnect_attempts;
          LOG_PRINT(args.get_log_level(), LogLevel::ERROR,
                    "ERROR: Disconnected from MPD, reconnecting in {} "
                    "milliseconds (attempt {})...",
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        RECONNECT_INTERVAL)
                        .count(),
                    reconnect_attempts);
          message = std::format("connection attempt {}...", reconnect_attempts);
        }
      }
    } else if (!cli.is_ok() && reconnect_attempts >= MAX_RECONNECT_ATTEMPTS) {
      LOG_PRINT(LogLevel::ERROR, LogLevel::ERROR,
                "ERROR: Failed to reconnect after {} attempts, stopping...",
                MAX_RECONNECT_ATTEMPTS);
      break;
    } else if (cli.is_ok() && message) {
      message.reset();
    }

    cli.update();
    if (cli.needs_auth()) {
      message.reset();
      do_auth();
    }
    disp->update(cli, args);

    // draw
    BeginDrawing();
    ClearBackground(CLEAR_BG_COLOR);
    disp->draw(cli, args);
    if (message) {
      DrawRectangle(0, 20, GetScreenWidth(), 20, BLACK);
      DrawText(message->c_str(), 0, 20, 20, WHITE);
    }
    EndDrawing();
  }

  disp.reset();

  CloseWindow();

  return 0;
}
