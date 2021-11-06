/*
 * clipboard.h
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <bits/types/FILE.h>
#include <iostream>
#include <ostream>

bool wait_for_clipboard_change(std::string*, std::string*);
void get_clipboard_content(std::string*, std::string*);
void write_clipboard_to_file(std::string, FILE*);
void find_target(std::ostream*);
std::string find_target_mime_type(std::string);
void append_to_history(std::string, std::string, std::string);

#endif /* !CLIPBOARD_H */
