/*
 * config.h
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

/*
 * Command to run to grab clipboard content. It is recommended that `xclip` is
 * used.
 *
 * Default: `xclip -selection clipboard -o`
 */
static const char *CLIPBOARD_COMMAND = "xclip -selection clipboard -o";

/*
 * Maximum clipboard history size. Currently does nothing.
 *
 * Default: `256`
 */
static const int CLIPBOARD_HIST_SIZE = 256;

/*
 * Clipboard mime type priority list. Clipman will try to find if the clipboard
 * content contains a mime type lower in the priority list FIRST.
 *
 * Default: "image/png", "image/jpg", "text/plain", "UTF8_STRING"
 */
static const char *TARGET_PRIORITY_LIST[] = {
    "image/png", // highest priority
    "image/jpg",
    "text/plain",
    "UTF8_STRING" // lowest priority
};

/*
 * Specifies which mime types are binary formats. This is used so clipboard
 * binary is read with `fread` and clipboard text is read with `fgets`.
 *
 * Default: "image/png", "image/jpg"
 */
static const char *BINARY_MIME_TYPE_LIST[] = {
    "image/png"
    "image/jpg"
};

/*
 * Default path to store clipboard history (when argument is not passed in)
 *
 * Default: `"$HOME/.cache/clipman"`
 */
static const char *DEFAULT_HIST_PATH = "$HOME/.cache/clipman";

#endif /* !CONFIG_H */
