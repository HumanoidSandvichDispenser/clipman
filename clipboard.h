/*
 * clipboard.h
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <iostream>

bool wait_for_clipboard_change(std::string*, std::string*);
void get_clipboard_content(std::string*, std::string*);
std::string get_stdout(const char*);
std::string find_target(std::string);
void append_to_history(std::string, std::string, std::string);
std::string md5sum(std::string);

#endif /* !CLIPBOARD_H */
