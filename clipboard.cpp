/*
 * clipboard.cpp
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "clipboard.h"
#include "config.h"
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <openssl/md5.h>
#include <ostream>
#include <memory>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <chrono>
#include <iomanip>

#define CLIPBOARD_COMMAND "xclip -selection clipboard -o"


bool wait_for_clipboard_change(std::string* out, std::string* targets) {
    FILE* stream = popen("clipnotify", "r");
    pclose(stream); // wait for clipnotify to end (when clipboard changes)
    get_clipboard_content(out, targets);
    return true;
}

void get_clipboard_content(std::string* out, std::string* targets) {
    *out = get_stdout(CLIPBOARD_COMMAND);
    *targets = get_stdout("xclip -selection clipboard -o -t TARGETS");
}

std::string get_stdout(const char* exec) {
    // use const char* exec because popen is a C function
    std::array<char, 128> buf;
    std::string res = "";
    std::unique_ptr<FILE, decltype(&pclose)> stream(popen(exec, "r"), pclose);

    if (!stream) {
        throw std::runtime_error("Unable to popen");
    }
    
    while (fgets(buf.data(), buf.size(), stream.get()) != nullptr) {
        res += buf.data();
    }

    return res;
}

std::string find_target(std::string curr_targets) {
    for (std::string target : TARGET_PRIORITY_LIST) {
        if (curr_targets.find(target) != std::string::npos) {
            return target;
        }
    }

    return "";
}

void append_to_history(std::string content, std::string target, std::string path) {
    // TODO: Replace naming convention to hashes. This means previously copied
    // items won't reappear on the list, but their modify time will change.
    std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    //long int timestamp = ms.count();
    std::string hash = md5sum(content);
    
    std::ofstream outfile(path + "/" + hash + ".clip");
    outfile << target << std::endl << content;

    outfile.close();
}

std::string md5sum(std::string str) {
    unsigned char* res_c = MD5((unsigned char*)str.c_str(), str.size(), NULL);
    std::stringstream stream;
    std::string digest = "";
    for (int i = 0; i < 16; i++) {
        stream << std::hex << (int)res_c[i];
    }
    return stream.str();
}
