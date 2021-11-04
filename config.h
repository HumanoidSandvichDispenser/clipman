/*
 * config.h
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

/*
 * Command to run to grab clipboard content
 *
 * Default: `xclip -selection clipboard -o`
 */
static const std::string CLIPBOARD_COMMAND = "xclip -selection clipboard -o";

/*
 * Maximum clipboard history size
 *
 * Default: `256`
 */
static const int CLIPBOARD_HIST_SIZE = 256;

/*
 * Clipboard target atom priority list.
 *
 * Default: "image/png", "image/jpg", "text/html", "text/plain", "UTF8_STRING"
 */
static const std::string TARGET_PRIORITY_LIST[] = {
    "image/png",
    "image/jpg",
    "text/html",
    "text/plain",
    "UTF8_STRING"
};

/*
 * Adds an item to the clipboard even if a target was not found for it.
 *
 * Default: `false`
 */
static const bool FORCE_ADD_TO_HIST = false;

/*
 * Default path to store clipboard history (when argument is not passed in)
 *
 * Default: `$HOME/.cache/clipman`
 */
static const std::string DEFAULT_HIST_PATH = "$HOME/.cache/clipman";

#endif /* !CONFIG_H */
