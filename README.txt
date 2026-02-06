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
  -h | --help : show this usage text
  --version : show the version of this program
  --host=<ip_addr> : ip address of mpd server
  --port=<port> : port of mpd server (default 6600)
  --disable-all-text : disables showing all text
  --disable-show-title : disable showing song title
  --disable-show-artist : disable showing song artist
  --disable-show-album : disable showing song album
  --disable-show-filename : disable showing song filename
  --disable-show-remaining : disables showing remaining time
  --disable-show-percentage : disable showing song percentage
  --align-text-right : Aligns the text to the right
  --pprompt : prompt for password on program start
  --pfile=<filename> : get password from specified file
  --no-scale-fill : don't scale fill the album art to the window
  --align-album-art-left : align the album art to the left
  --align-album-art-right: align the album art to the right
  --log-level=<level> : set the log level (ERROR, WARNING, DEBUG, VERBOSE)
  --bg-opacity=<percentage> : set the text bg opacity by percentage (decimal point allowed)
  --default-font-filename=<font_filename> : set the default font
  --force-default-font : Only use the default font (mutually exclusive with next option)
  --force-default-font-ascii : Only use the default font for ascii text (mutually exclusive with previous option)
  --blacklist-font-str=<string> : blacklist fonts that have <string> in its filename (use this option multiple times to add more strings to check)
  --remaining-force-default-raylib-font : force the remaining time text to always use Raylib's default font
  --font-scale-factor=<factor> : Sets the factor to scale the font size with (default 1.0)

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

When using the Makefile, define the environment variable `USE_EXTERNAL_GLFW`,
and it will build with the system's GLFW when compiling Raylib.

When using CMake, define `-DUSE_EXTERNAL_GLFW=ON`, and it will use the system's
GLFW when compiling Raylib.
