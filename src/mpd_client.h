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

#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_MPD_CLIENT_H_

#include <bitset>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

// local includes
#include "constants.h"

class MPDClient {
 public:
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

  const std::string &get_song_title() const;
  const std::string &get_song_artist() const;
  const std::string &get_song_album() const;
  const std::string &get_song_filename() const;
  double get_song_duration() const;
  std::tuple<double, std::chrono::steady_clock::time_point> get_elapsed_time()
      const;
  const std::optional<std::vector<char> > &get_album_art() const;
  const std::string &get_album_art_mime_type() const;

  bool song_has_album_art() const;

  void request_data_update();
  void request_refetch_album_art();

 private:
  enum StatusEnum {
    SE_SUCCESS,
    SE_EAGAIN_ON_READ,
    SE_GENERIC_ERROR,
    SE_READ_TIMED_OUT,
    SE_WRITE_TIMED_OUT
  };

  constexpr static std::string status_to_str(StatusEnum val) {
    switch (val) {
      case SE_SUCCESS:
        return "SE_SUCCESS";
      case SE_EAGAIN_ON_READ:
        return "SE_EAGAIN_ON_READ";
      case SE_GENERIC_ERROR:
        return "SE_GENERIC_ERROR";
      case SE_READ_TIMED_OUT:
        return "SE_READ_TIMED_OUT";
      case SE_WRITE_TIMED_OUT:
        return "SE_WRITE_TIMED_OUT";
      default:
        return "UNKNOWN";
    }
  }

  // 0 - invalid state
  // 1 - initial state
  // 2 - successful ping
  // 3 - successful status
  // 4 - waiting on read
  // 5 - permission/auth required
  // 6 - successful "currentsong"
  // 7 - non-blocking set
  // 8 - need to fetch album art
  // 9 - current song no "readpicture"
  // 10 - current song no "albumart"
  // 11 - failed to fetch album art
  std::bitset<64> flags;
  LogLevel level;
  std::optional<uint32_t> host_ip_value;
  uint16_t host_port;
  int tcp_socket;

  // current song info
  std::string song_title;
  std::string song_artist;
  std::string song_album;
  std::string song_filename;
  std::chrono::steady_clock::time_point elapsed_time_point;
  double elapsed_time;
  double song_duration;
  std::optional<std::vector<char> > album_art;
  std::optional<std::vector<char> > dummy_album_art_ref;
  std::string album_art_mime_type;
  std::optional<size_t> album_art_offset;
  size_t album_art_expected_size;

  std::tuple<StatusEnum, std::string> write_read(std::string to_send);

  void cleanup_close_tcp();

  void parse_for_song_info(const std::string &buf);
  void parse_for_album_art(const std::string &buf);
};

#endif
