/*
 * main.h
 * Copyright (C) 2021 sandvich <sandvich@arch>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef MAIN_H
#define MAIN_H

#include "opts.h"
#include <string>

static bool is_enabled = true;

bool is_another_process_running();
void handle_sigusr(void);
void expand_env(std::string*);
void parse_args(int, char**, opts*);
int main(int, char**);

#endif /* !MAIN_H */
