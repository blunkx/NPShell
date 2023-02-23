#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

#include "command.h"
using namespace std;

typedef enum BUILT_IN_COM_T
{
    NOT_BUILT_IN,
    SETENV,
    PRINTENV,
    EXIT,
} BUILT_IN_COM_E;

void parser(string &input);

#endif
