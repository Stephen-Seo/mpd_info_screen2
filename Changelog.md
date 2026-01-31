# Upcoming Changes

# Version 1.5.3

Attempt to fix text rendering bug where some characters in the display text
don't display properly.

# Version 1.5.2

Switch from using `#include <print>` directly to `PrintHelper` for platforms
that have a version of a C++ compiler that doesn't support C++23.  
It is expected to have support for C++20.

Fixes to Makefile to support builds on other systems.

Add a Github workflow that automatically runs the `unittest` on git-push.

# Version 1.5.1

Fix "glitching" text display on song change.

Current behavior is to keep displaying the info text, and only changing them
when the current song has changed, hopefully with a "seamless transition". This
should be done in a way to prevent any graphical issues when the current song
changes.

Fix potential crash/undefined-behavior by ensuring the rendered text's font size
never goes negative (or zero).

# Version 1.5

Add a Makefile and CMake option to build `mpd_info_screen2` with the system's
GLFW instead of building the GLFW bundled with Raylib.

When using the Makefile, define the environment variable `USE_EXTERNAL_GLFW`,
and it will build with the system's GLFW when compiling Raylib.

When using CMake, define `-DUSE_EXTERNAL_GLFW=ON`, and it will use the system's
GLFW when compiling Raylib.

# Version 1.4

Add `--remaining-force-default-raylib-font`, which forces the remaining time
text to always use the default Raylib font.

Refactoring to make `mpd_info_screen2` more efficient; Don't reload
FontConfig's configuration every load (`FcInitLoadConfigAndFonts()`), and
instead run `FcInit()` and use the default configuration loaded from it.

# Version 1.3

Fix regression where remaining-time text's background does not update with the
remaining-time text.

Patched Raylib to disable any form of "busy wait", which should significantly
increase the efficiency of the program (less CPU cycles used).

# Version 1.2

Implement `--blacklist-font-str=<string>`, which allows the user to prevent
specific fonts from being loaded. This applies to the font's filename, and the
check is case-sensitive.

Fix regression where remaining-time display "glitches" every few seconds.

# Version 1.1

Implement automatic scaling of text to window size.  
(A larger window means larger text, as long as the text is not too large.)

Fix issues with resizing font/text with robust handling.

Round floating-point values to nearest integers when positioning/drawing texts.

# Version 1.0

Initial version with the following options:

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
