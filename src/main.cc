// local includes
#include "args.h"
#include "helpers.h"
#include "mpd_client.h"

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
    LOG_PRINT(LogLevel::VERBOSE, LogLevel::VERBOSE, "Client is NOT OK");
    return 2;
  } else {
    LOG_PRINT(LogLevel::VERBOSE, LogLevel::VERBOSE, "Client is OK");
  }

  InitWindow(800, 600, "mpd_info_screen2");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(5);

  while (!WindowShouldClose()) {
    // update
    cli.update();

    // draw
    BeginDrawing();
    ClearBackground(CLEAR_BG_COLOR);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
