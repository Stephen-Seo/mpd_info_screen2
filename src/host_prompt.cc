#include "host_prompt.h"

#include <raylib.h>

HostPrompt::HostPrompt() : selection(0), addr(), socket() {}

bool HostPrompt::update() {
  if (IsKeyPressed(KEY_DOWN)) {
    selection = selection + 1;
    if (selection > 1) {
      selection = 1;
    }
  } else if (IsKeyPressed(KEY_UP)) {
    selection = selection - 1;
    if (selection < 0) {
      selection = 0;
    }
  } else if (IsKeyPressed(KEY_BACKSPACE)) {
    switch (selection) {
      case 0:
        if (!addr.empty()) {
          addr.pop_back();
        }
        break;
      case 1:
        if (!socket.empty()) {
          socket.pop_back();
        }
        break;
      default:
        break;
    }
  } else if (IsKeyPressed(KEY_ENTER)) {
    return true;
  }

  int pressed = GetCharPressed();
  while (pressed != 0) {
    if (selection == 0) {
      const char *pressed_c = reinterpret_cast<const char *>(&pressed);
      while (pressed != 0) {
        addr.push_back(pressed_c[0]);
        pressed = (pressed >> 8) & 0x00FFFFFF;
      }
    } else if (selection == 1) {
      const char *pressed_c = reinterpret_cast<const char *>(&pressed);
      while (pressed != 0) {
        socket.push_back(pressed_c[0]);
        pressed = (pressed >> 8) & 0x00FFFFFF;
      }
    }
    pressed = GetCharPressed();
  }

  return false;
}

void HostPrompt::draw() {
  DrawRectangle(0, 0, GetScreenWidth(), HOST_PROMPT_LINE_SIZE,
                selection == 0 ? WHITE : BLACK);
  DrawText("Host Addr: ", 0, 0, HOST_PROMPT_LINE_SIZE,
           selection == 0 ? BLACK : WHITE);
  int width = MeasureText("Host Addr: ", HOST_PROMPT_LINE_SIZE);
  DrawText(addr.c_str(), width, 0, HOST_PROMPT_LINE_SIZE,
           selection == 0 ? BLACK : WHITE);

  DrawRectangle(0, HOST_PROMPT_LINE_SIZE, GetScreenWidth(),
                HOST_PROMPT_LINE_SIZE, selection == 1 ? WHITE : BLACK);
  DrawText("Host Socket: ", 0, HOST_PROMPT_LINE_SIZE, HOST_PROMPT_LINE_SIZE,
           selection == 1 ? BLACK : WHITE);
  width = MeasureText("Host Socket: ", HOST_PROMPT_LINE_SIZE);
  DrawText(socket.c_str(), width, HOST_PROMPT_LINE_SIZE, HOST_PROMPT_LINE_SIZE,
           selection == 1 ? BLACK : WHITE);
}

const std::string &HostPrompt::get_addr() const { return addr; }

const std::string &HostPrompt::get_socket() const { return socket; }
