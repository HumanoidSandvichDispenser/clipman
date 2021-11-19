/*
 * clipboard.cpp
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "clipboard.h"
#include "config.h"
#include "utils.h"
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <chrono>
#include <iomanip>


bool wait_for_clipboard_change(std::string* out, std::string* targets) {
    FILE* stream = popen("clipnotify", "r");
    pclose(stream); // wait for clipnotify to end (when clipboard changes)
    get_clipboard_content(out, targets);
    return true;
}

void get_clipboard_content(std::string* out, std::string* targets) {
    *targets = get_stdout("xclip -selection clipboard -o -t TARGETS");
    std::string target = find_target_mime_type(*targets);
    *out = get_stdout((std::string(CLIPBOARD_COMMAND) + " -t " + target).c_str());
}

/*
 * Like get_stdout, but written specfically to read and write binary data.
 */
void write_clipboard_to_file(std::string exec, FILE *filestream) {
    char buf[128];
    std::string res = "";
    std::unique_ptr<FILE, decltype(&pclose)> pstream(popen(exec.c_str(), "r"), pclose);

    while (true) {
        int buf_size = std::fread(buf, 1, 128, pstream.get());
        if (buf_size <= 0) {
            // no more content to read from buffer. stop!
            break;
        }
        std::fwrite(buf, 1, 128, filestream);
    }
}

std::string find_target_mime_type(std::string curr_targets) {
    for (std::string target : TARGET_PRIORITY_LIST) {
        if (curr_targets.find(target) != std::string::npos) {
            return target;
        }
    }

    return "";
}

void append_to_history(std::string content, std::string target_mime, std::string path) {
    std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    std::string hash = md5sum(content);
    std::string filename = (path + "/" + hash + ".clip");

    // binary data formats will have their bytes directly written with fread/fwrite instead of fgets
    if (target_mime == "image/png") {
        std::unique_ptr<FILE, decltype(&fclose)> outfile(fopen(filename.c_str(), "wb"), fclose);
        write_clipboard_to_file(std::string(CLIPBOARD_COMMAND) + " -t " + target_mime, outfile.get());
        // since outfile is a unique_ptr, it will automatically dispose
    } else {
        std::ofstream outfile(filename);
        outfile << content;
        outfile.close();
    }
    
    std::ofstream headerfile(filename + "h");
    headerfile << target_mime;
    headerfile.close();
}
