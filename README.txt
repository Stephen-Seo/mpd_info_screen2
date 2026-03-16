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
  --whitelist-font-str=<string> : whitelist fonts that have <string> in its filename (use this option multiple times to add more strings to check; if a font matches ANY strings in the whitelist, it is allowed)
  --remaining-force-default-raylib-font : force the remaining time text to always use Raylib's default font
  --font-scale-factor=<factor> : Sets the factor to scale the font size with (default 1.0)
  --remaining-font-scale-factor=<factor> : Sets the factor to scale the remaining (remaining time and elapsed percentage) text's font size with (default 1.0)
  --h-toggles-text : Make the "H" key toggle displaying text instead of only hiding while pressed
  --background-color=<value> : Sets the grayscale color of the background (between 0.0 and 1.0; black and white)

--------------------------------------------------------------------------------
    Man Page
--------------------------------------------------------------------------------

The html version of the man page of mpd_info_screen2 can be found here:
https://stephen-seo.github.io/mpd_info_screen2/

--------------------------------------------------------------------------------
    Compiling: Makefile
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

Define `FORCE_DEBUG_FLAG` and even release builds will use `-g` passed to the
C++ compiler.

--------------------------------------------------------------------------------
    Compiling: CMake
--------------------------------------------------------------------------------

Same dependencies as the Makefile.

Set `-DUSE_EXTERNAL_GLFW=On` to use the system's glfw when building Raylib.

Set `-DFORCE_DEBUG_FLAG=On` to use `-g` even in release builds.

--------------------------------------------------------------------------------
    Compiling: CMake Bundled
--------------------------------------------------------------------------------

Using "bundled/CMakeLists.txt".

The CMake bundled build is configured to bundle most dependencies.

meson, ninja, cmake, python, and gperf are build dependencies.

`-DCMAKE_BUILD_TYPE=Release` is recommended.

When cross compiling, use a command like the following example:

CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ cmake -S bundled -B buildAArch64 -DCMAKE_BUILD_TYPE=Release -DCROSS_CC=aarch64-linux-gnu-gcc -DCROSS_CXX=aarch64-linux-gnu-g++ -DCROSS_HOST=aarch64-linux-gnu -DCROSS_AR=aarch64-linux-gnu-ar -DCROSS_RANLIB=aarch64-linux-gnu-ranlib -DCROSS_SYSTEM=linux -DCROSS_CPU=aarch64

--------------------------------------------------------------------------------
    Maintenance
--------------------------------------------------------------------------------

Please read the "Maintenance" section in
https://github.com/Stephen-Seo/PoorMansAnubis/blob/main/README.txt
before reading this section.

Differences between PoorMansAnbuis' bundled handling and this project:

 - The "bundled" version of mpd_info_screen2 does all of its dependency
   handling with "cmake".

 - SHA256 hashes are compared by using "grep" instead of using a separate
   SHA256SUM text file.

 - "cmake" in some cases may be more difficult to manage than a Makefile, but
   the existing bundled/CMakeLists.txt can be examined before updating it, as
   each dependency is handled in a similar manner.
