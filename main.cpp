/*
 * main.cpp
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Clipman - simple clipboard manager
 * Copyright © 2021 HumanoidSandvichDispenser
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "clipboard.h"
#include "config.h"
#include "opts.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <wordexp.h>
#include <signal.h>
#include <sys/file.h>
#include <errno.h>


void handle_sigusr(int signum) {
    if (signum == SIGUSR2) {
        is_enabled = !is_enabled;
        std::cout << "toggled" << std::endl;
    } else if (signum == SIGRTMIN + 1) {
        is_enabled = false;
        std::cout << "disabled" << std::endl;
    } else if (signum == SIGRTMIN + 2) {
        is_enabled = true;
        std::cout << "enabled" << std::endl;
    }
}

void expand_env(std::string *str) {
    std::string res;
    wordexp_t p;
    char **w;
    wordexp(str->c_str(), &p, 0);
    w = p.we_wordv;
    for (size_t i = 0; i < p.we_wordc; i++) {
        res += w[i];
    }
    *str = res;
}

void parse_args(int argc, char **argv, opts *options) {
    options->path = DEFAULT_HIST_PATH;

    // replace this with actual arg parsing if we do need more arguments soon
    if (argc > 1 && false) {
        options->path = argv[1];
    }

    expand_env(&(options->path));
}

int main(int argc, char **argv) {
    int pid_file = open("/var/run/clipman.pid", O_CREAT | O_RDWR);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);

    if (rc) {
        if (errno == EWOULDBLOCK) {
            std::cerr << "Another instance of clipman is running! Remove /var/run/clipman.pid if no other instance is running." << std::endl;
            return 1;
        }
    }

    opts *options, opts_v;
    options = &opts_v;

    parse_args(argc, argv, options);

    // remove me
    std::cout << "options->path = " << options->path << std::endl;

    // catch SIGUSR2 for toggle, SIGRTMIN+1 for disable, and SIGRTMIN+2 for
    // enable
    signal(SIGUSR2, handle_sigusr);
    signal(SIGRTMIN + 1, handle_sigusr);
    signal(SIGRTMIN + 2, handle_sigusr);

    std::string clip_content = "";
    std::string clip_targets = "";
    std::string previous_clip_content = "";
    while (wait_for_clipboard_change(&clip_content, &clip_targets)) {
        if (!is_enabled) {
            std::cout << "caught event, but releasing since is_enabled is false." << std::endl;
            continue;
        }

        if (clip_content != previous_clip_content) {
            // clipboard has changed; register new content.
            previous_clip_content = clip_content;
            std::string target_mime = find_target_mime_type(clip_targets);
            if (target_mime != "")
                append_to_history(clip_content, target_mime, options->path);
        }
    }
}
