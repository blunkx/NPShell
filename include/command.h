
#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

typedef enum PIPE_TYPE_T
{
    NO_PIPE,
    PIPE,
    ERR_PIPE,
    F_RED_PIPE,
    NUM_PIPE,
    ERR_NUM_PIPE,

} PIPE_TYPE_E;

class command
{
private:
public:
    PIPE_TYPE_E pipe_type;
    int pipe_num = 0;
    bool is_exe = false;
    bool is_piped = false;
    int fd[2];
    vector<string> cmd;
    string which_type();
};

void init_env();
void print_env(const char *const para);
void exe_bin(vector<command> &cmds);

void print_cmds(vector<command> cmds);
void init_pipe(int *fd);
#endif
