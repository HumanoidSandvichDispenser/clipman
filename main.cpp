/*
 * main.cpp
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "main.h"
#include "clipboard.h"
#include "config.h"
#include "opts.h"
#include <iostream>
#include <string>
#include <vector>
#include <wordexp.h>


void expand_env(std::string* str) {
    std::string res;
    wordexp_t p;
    char** w;
    wordexp(str->c_str(), &p, 0);
    w = p.we_wordv;
    for (size_t i = 0; i < p.we_wordc; i++) {
        res += w[i];
    }
    *str = res;
}

void parse_args(int argc, char** argv, opts* options) {
    options->path = DEFAULT_HIST_PATH;

    if (argc > 1 && false) {
        options->path = argv[1];
    }
    std::cout << options->path << std::endl;
    expand_env(&(options->path));
}

int main(int argc, char** argv) {
    opts *options, opts_v;
    options = &opts_v;

    parse_args(argc, argv, options);

    std::cout << "options->path = " << options->path << std::endl;

    std::string clip_content = "";
    std::string clip_targets = "";
    std::string previous_clip_content = "";
    while (wait_for_clipboard_change(&clip_content, &clip_targets)) {
        if (clip_content != previous_clip_content) {
            // clipboard has changed; register new content.
            previous_clip_content = clip_content;
            std::string target = find_target(clip_targets);
            if (target != "")
                append_to_history(clip_content, target, options->path);
            std::cout << "Clipboard has changed." << std::endl;
            std::cout << "Clipboard target: " << target << std::endl;
            std::cout << "New content: " << clip_content << std::endl;
        }
    }
}
