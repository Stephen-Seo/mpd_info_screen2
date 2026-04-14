# Upcoming Changes

Implement options `--y-offset-top=...` and `--y-offset-bottom=...`.

The "top" variant aligns the text to the top of the screen with an offset.

The "bottom" variant is the same as the default behavior if the value given is
0 . Otherwise, the text is offset from the bottom of the screen.

# Version 1.19.5

Update bundled dependencies:

 - libpng 1.6.56 -> 1.6.57

# Version 1.19.4

Update bundled dependencies:

 - harfbuzz 14.0.0 -> 14.1.0

# Version 1.19.3

Update bundled dependencies:

 - harfbuzz 13.2.1 -> 14.0.0
 - wayland-protocols 1.47 -> 1.48

# Version 1.19.2

Update bundled dependencies:

 - libpng 1.6.55 -> 1.6.56

# Version 1.19.1

Update bundled dependencies:

 - libexpat 2.7.4 -> 2.7.5
 - libfreetype 2.14.2 -> 2.14.3
 - harfbuzz 13.1.1 -> 13.2.1
 - wayland 1.24.0 -> 1.25.0

# Version 1.19

Implement `--scale-text-by-width`, `--scale-text-by-height`,
`--scale-text-by-wh-min`, and `--scale-text-by-wh-max`.  
Note that by default, `--scale-text-by-width` is in effect.

# Version 1.18.2

Attempt fix of failing to load an album art image for the currently playing
song. On failure, `mpd_info_screen2` will attempt to reload the album art from
MPD up to 5 times. If it still fails to load, the album art will no longer be
loaded again for the current song. When the next song plays, `mpd_info_screen2`
should re-attempt to load the album art image.

The unit test should now successfully build for arm 32-bit targets by
explicitly linking against `atomic`.

# Version 1.18.1

Actually fix setting of the executable when building from a directory outside
of the source directory with CMake. (The previous version only fixed this with
the standard CMakeLists.txt but not the bundled/CMakeLists.txt .)

# Version 1.18

Fix drawing of certain texts based on background color. This should fix problems
like the `--prompt` text not being visible.

Fix setting the version of the executable when building from a directory outside
of the source directory with CMake.

Impl `--text-fg-color` and `--text-bg-color`.

# Version 1.17.1

Fix other informational text to be black if `--background-color=...` value is
greater than 0.5 .

# Version 1.17

Add `--background-color=<value>` that takes a value between 0.0 and 1.0 to
determine the background color (between black and white).

# Version 1.16

Add `--h-toggles-text` option to change the behavior of the "H" key. Check the
README.txt and/or man page for more details.

# Version 1.15.2

Minor tweaks to album-art-fetching code to more robustly handle error during
fetching.

# Version 1.15.1

Update bundled dependencies:

 - libxml2 (2.15.1 -> 2.15.2)
 - harfbuzz (12.3.2 -> 13.0.1)

# Version 1.15

Refactor how font searching is handled when a default font is specified, such
that the default font is preferred if it is one of the eligible fonts for a
line of text.

In other words, if a default font is set, then it is used most of the time even
for non-ascii text (as long as the default font can render all characters in
the text).

Note that `--force-default-font-ascii` will not be removed because `font_config`
usually returns paths to fonts with absolute paths, but a user can specify a
font with `--default-font-filename=<filename>` with a relative path.

# Version 1.14.2

Update bundled dependency freetype to 2.14.2 .

Minor tweak to bundled CMakeLists.txt .

# Version 1.14.1

Minor fixes/refactorings.

# Version 1.14

More robust handling of when a font is not found for the text to be displayed.

Reconnect on a 5 second interval if connection to MPD is lost. Up to 5
reconnection attempts are made, after which the program stops if reconnection
fails.

Add font-name whitelisting via args.

# Version 1.13

Minor fixes/refactorings.

On failure to find appropriate font for text, default to Raylib's default font.

# Version 1.12

Fix bundled build on systems without `bison` installed.

# Version 1.11

Minor refactorings/improvements.

Create bundled build in `bundled/CMakeLists.txt`. Should support cross
compilation (see the README).

Minor refactoring to `CMakeLists.txt`.

# Version 1.10

Implement `--remaining-font-scale-factor=<factor>` which only affects the
remaining time text (remaining time and elapsed percentage). This option has
precedence over `--font-scale-factor=<factor>`.

Fix bad spacing between texts (cleanly displaying each text such that there are
no gaps between them).

# Version 1.9

Implement `--font-scale-factor=<factor>`, and add man page entry for it.

Implement `--align-album-art-left` and `--align-album-art-right`, and add man
page entries for them.

# Version 1.8

Add `--align-text-right`.

Minor fixes/refactorings.

# Version 1.7.1

Fix linking order in Makefile.

# Version 1.7

Use `git describe --long --tags` to set the "version string".

Refactor `src/print_helper.h`: remove unnecessary functions.

Attempt to fix ipv4 address parsing for big-endian systems.

Removed some unnecessary commented out code.

# Version 1.6.2

Minor refactorings:

 - More robust setting faster FPS when `--pprompt` is prompted.
 - Change some literals in the code to constants (usually in src/constants.h).
 - Remove some unnecessary commented out code in `src/mpd_display.cc`
 - Remove some debug prints related to font/text loading.

# Version 1.6.1

Fix `mpd_info_screen2` not returning success when using `--version`.

# Version 1.6

Added `FORCE_DEBUG_FLAG` for Makefile and CMakeLists.txt:

Define `FORCE_DEBUG_FLAG` before using the Makefile to force the compiler to
use the `-g` flag.

Use `-DFORCE_DEBUG_FLAG=On` when using CMake to force the compiler to use the
`-g` flag.

Note that the `-g` flag adds debugging symbols to the output binary.

Actually implement `--no-scale-fill`.

Add `--version`.

Create "man page" for `mpd_info_screen2`.

# Version 1.5.4

The changes in this version mostly increase the efficiency of
`mpd_info_screen2`:

Avoid re-usage of MeasureTextEx (provided by Raylib) where possible to increase
efficiency.

Use cached texts instead of MPDClient's current texts when using MeasureTextEx.

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
