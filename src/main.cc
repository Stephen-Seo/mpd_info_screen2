// local includes
#include "args.h"
#include "helpers.h"
#include "mpd_client.h"
#include "signal_handler.h"

// Standard library includes
#include <atomic>
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

  while (!WindowShouldClose() &&
         !IS_SIGNAL_HANDLED.load(std::memory_order_relaxed)) {
    // update
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
