# Upcoming Changes

Implement `--blacklist-font-str=<string>`, which allows the user to prevent
specific fonts from being loaded. This applies to the font's filename, and the
check is case-sensitive.

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
