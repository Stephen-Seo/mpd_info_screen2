#include <raylib.h>

constexpr Color CLEAR_BG_COLOR = {50, 50, 50, 255};

int main(int argc, char **argv) {
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
