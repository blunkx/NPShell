
#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void init_env();
void print_env(const char *const para);
void exe_bin(vector<vector<string>> cmds);

#endif
