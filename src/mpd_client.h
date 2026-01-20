#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// local includes
#include "constants.h"

class MPDClient {
 public:
  enum Event { OK, PassRequested };

  MPDClient(std::string host_ip, uint16_t host_port, LogLevel level);
  ~MPDClient();

  // No copy
  MPDClient(const MPDClient &) = delete;
  MPDClient &operator=(const MPDClient &) = delete;

  // Allow move
  MPDClient(MPDClient &&);
  MPDClient &operator=(MPDClient &&);

  void reset_connection();
  bool is_ok() const;

  void update();
  Event get_event();

 private:
  // 0 - invalid state
  // 1 - initial state
  // 2 - successful ping
  // 3 - successful status
  std::bitset<64> flags;
  std::vector<Event> events;
  LogLevel level;
  std::optional<uint32_t> host_ip_value;
  uint16_t host_port;
  int tcp_socket;
};

#endif
