#include "host_prompt.h"

#include <raylib.h>

HostPrompt::HostPrompt() : selection(0), addr(), socket() {}

bool HostPrompt::update() {
  if (IsKeyPressed(KEY_DOWN)) {
    selection = selection + 1;
    if (selection > 1) {
      // wrap around.
      selection = 0;
    }
  } else if (IsKeyPressed(KEY_UP)) {
    selection = selection - 1;
    if (selection < 0) {
      // wrap around.
      selection = 1;
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
  if (selection == 0) {
    DrawRectangle(0, 0, GetScreenWidth(), HOST_PROMPT_LINE_SIZE, WHITE);
    DrawText("Host Addr: ", 0, 0, HOST_PROMPT_LINE_SIZE, BLACK);
    int width = MeasureText("Host Addr: ", HOST_PROMPT_LINE_SIZE);
    DrawText(addr.c_str(), width, 0, HOST_PROMPT_LINE_SIZE, BLACK);

    DrawRectangle(0, HOST_PROMPT_LINE_SIZE, GetScreenWidth(),
                  HOST_PROMPT_LINE_SIZE, BLACK);
    DrawText("Host Socket: ", 0, HOST_PROMPT_LINE_SIZE, HOST_PROMPT_LINE_SIZE,
             WHITE);
    width = MeasureText("Host Socket: ", HOST_PROMPT_LINE_SIZE);
    DrawText(socket.c_str(), width, HOST_PROMPT_LINE_SIZE,
             HOST_PROMPT_LINE_SIZE, WHITE);
  } else if (selection == 1) {
    DrawRectangle(0, 0, GetScreenWidth(), HOST_PROMPT_LINE_SIZE, BLACK);
    DrawText("Host Addr: ", 0, 0, HOST_PROMPT_LINE_SIZE, WHITE);
    int width = MeasureText("Host Addr: ", HOST_PROMPT_LINE_SIZE);
    DrawText(addr.c_str(), width, 0, HOST_PROMPT_LINE_SIZE, WHITE);

    DrawRectangle(0, HOST_PROMPT_LINE_SIZE, GetScreenWidth(),
                  HOST_PROMPT_LINE_SIZE, WHITE);
    DrawText("Host Socket: ", 0, HOST_PROMPT_LINE_SIZE, HOST_PROMPT_LINE_SIZE,
             BLACK);
    width = MeasureText("Host Socket: ", HOST_PROMPT_LINE_SIZE);
    DrawText(socket.c_str(), width, HOST_PROMPT_LINE_SIZE,
             HOST_PROMPT_LINE_SIZE, BLACK);
  }
}

const std::string &HostPrompt::get_addr() const { return addr; }

const std::string &HostPrompt::get_socket() const { return socket; }
