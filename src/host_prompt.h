#ifndef COM_SEODISPARATE_MPD_INFO_SCREEN2_HOST_PROMPT_H_
#define COM_SEODISPARATE_MPD_INFO_SCREEN2_HOST_PROMPT_H_

#include <cstdint>
#include <string>

constexpr int HOST_PROMPT_LINE_SIZE = 18;

class HostPrompt {
 public:
  HostPrompt();

  /// Returns true if finished.
  bool update();
  void draw();

  const std::string &get_addr() const;
  const std::string &get_socket() const;

 private:
  int_fast8_t selection;
  std::string addr;
  std::string socket;
};

#endif
