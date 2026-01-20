// local includes
#include "args.h"

// third-party includes
#include <raylib.h>

constexpr Color CLEAR_BG_COLOR = {50, 50, 50, 255};

int main(int argc, char **argv) {
  Args args(argc, argv);

  if (args.is_error()) {
    args.print_usage();
    return args.get_flags().test(8) ? 0 : 1;
  }

  InitWindow(800, 600, "mpd_info_screen2");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(5);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(CLEAR_BG_COLOR);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
