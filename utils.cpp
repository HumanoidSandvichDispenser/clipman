/*
 * utils.cpp
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "utils.h"
#include "config.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <openssl/md5.h>
#include <array>
#include <sys/stat.h>
#include <cerrno>
#include <errno.h>
#include <stdio.h>


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

std::string md5sum(std::string str) {
    unsigned char* res_c = MD5((unsigned char*)str.c_str(), str.size(), NULL);
    std::stringstream stream;
    std::string digest = "";
    for (int i = 0; i < 16; i++) {
        stream << std::hex << (int)res_c[i];
    }
    return stream.str();
}

void mkcachedir() {
    if (mkdir(DEFAULT_HIST_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        if (errno == EEXIST) {
            // directory already exists.
            return;
        } else {
            // an unexpected error occurred!
            std::cerr << "cannot create directory error: " << errno << std::endl;
            throw errno;
        }
    }
}
