#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
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

  bool needs_auth() const;
  void attempt_auth(std::string passwd);

  void update();
  Event get_event();

 private:
  enum StatusEnum { SE_SUCCESS, SE_EAGAIN_ON_READ, SE_GENERIC_ERROR };

  // 0 - invalid state
  // 1 - initial state
  // 2 - successful ping
  // 3 - successful status
  // 4 - waiting on read
  // 5 - permission/auth required
  // 6 - successful "currentsong"
  std::bitset<64> flags;
  std::vector<Event> events;
  LogLevel level;
  std::optional<uint32_t> host_ip_value;
  uint16_t host_port;
  int tcp_socket;

  std::tuple<StatusEnum, std::vector<char> > write_read(std::string to_send);

  void cleanup_close_tcp();
};

#endif
