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
