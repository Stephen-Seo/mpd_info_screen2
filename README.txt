--------------------------------------------------------------------------------
    MPD_INFO_SCREEN 2
--------------------------------------------------------------------------------

This program has reached feature parity with mpd_info_screen as of 2026-01-28.

mpd_info_screen2 should have some extra features in addition to those found in
mpd_info_screen.

The original mpd_info_screen can be found here:

https://crates.io/crates/mpd_info_screen
https://github.com/Stephen-Seo/mpd_info_screen
https://git.seodisparate.com/gitweb/?p=mpd_info_screen;a=summary

Note that the original mpd_info_screen uses the Rust library "ggez" to handle
the graphics. mpd_info_screen2 instead relies on "Raylib".

--------------------------------------------------------------------------------
    Args
--------------------------------------------------------------------------------

Usage:
  --host=<ip_addr> : ip address of mpd server
  --port=<port> : port of mpd server (default 6600)
  --disable-all-text : disables showing all text
  --disable-show-title : disable showing song title
  --disable-show-artist : disable showing song artist
  --disable-show-album : disable showing song album
  --disable-show-filename : disable showing song filename
  --disable-show-remaining : disables showing remaining time
  --disable-show-percentage : disable showing song percentage
  --pprompt : prompt for password on program start
  --pfile=<filename> : get password from specified file
  --no-scale-fill : don't scale fill the album art to the window
  --log-level=<level> : set the log level (ERROR, WARNING, DEBUG, VERBOSE)
  --bg-opacity=<percentage> : set the text bg opacity by percentage (decimal point allowed)
  --default-font-filename=<font_filename> : set the default font
  --force-default-font : Only use the default font (mutually exclusive with next option)
  --force-default-font-ascii : Only use the default font for ascii text (mutually exclusive with previous option)

--------------------------------------------------------------------------------
    Compiling
--------------------------------------------------------------------------------

Bash, make, git, curl, and cmake is required.

"fontconfig" is an implicit dependency.

"raylib" is built by the Makefile and CMakeLists.txt .

Just run "make" in the root directory of this project to build the raylib
dependency and this project.

It may be faster to use "make -j 4"
(assuming you have 4 threads in this example)
