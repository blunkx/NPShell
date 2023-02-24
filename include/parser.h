#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <fstream>
#include <regex>

#include <sys/stat.h>
#include <unistd.h>

#include "command.h"

using namespace std;

typedef enum BUILT_IN_COM_T
{
    NOT_BUILT_IN,
    SETENV,
    PRINTENV,
    EXIT,
} BUILT_IN_COM_E;

typedef enum TOKEN_TYPE_T
{
    F_REDIRE_SYM,
    PIPE_SYM,
    ERR_PIPE_SYM,
    ARG,
    CMD,
} TOKEN_TYPE_E;

void parser(string &input);

#endif
