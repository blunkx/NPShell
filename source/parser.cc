#include "parser.h"

// bool is_valid_command(const string &token)
// {

// }

BUILT_IN_COM_E is_built_in_command(const vector<string> tokens)
{
    /*
    setenv [var] [value]
    printenv [var]
    exit
    */
    if (tokens[0] == "setenv" && tokens.size() == 3)
        return SETENV;
    else if (tokens[0] == "printenv" && tokens.size() == 2)
        return PRINTENV;
    else if (tokens[0] == "exit" && tokens.size() == 1)
        return EXIT;
    else
        return NOT_BUILT_IN;
}
TOKEN_TYPE_E which_type(const string &token)
{
    struct stat buffer;
    if (stat(token.c_str(), &buffer) == 0)
    {
        return CMD;
    }
    else
        return ARG;
    // if (token == "setenv")
    //     return SETENV;
    // else if (token == "printenv")
    //     return PRINTENV;
    // else if (token == "exit")
    //     return EXIT;
    // else
    //     return NOT_BUILT_IN;
}

// bool is_pipe_or_rd(const string &token)
// {
//     if (token == "|" || token == "!" || token == ">")
//         return true;
//     else
//         return false;
// }
void split_by_pipe(vector<string> &tokens, vector<vector<string>> &cmds)
{
    vector<string>::iterator pre_end = tokens.begin();
    pre_end = tokens.begin();
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if (*it == "|" || *it == "!" || *it == ">")
        {
            if (pre_end != tokens.begin())
                pre_end += 1;
            vector<string> temp(pre_end, it);
            cmds.push_back(temp);
            pre_end = it;
        }
        else if (it == tokens.end() - 1)
        {
            if (*(pre_end) == "|" || *(pre_end) == "!" || *(pre_end) == ">")
                pre_end += 1;
            vector<string> temp(pre_end, tokens.end());
            cmds.push_back(temp);
        }
    }
}
void split_by_number_pipe(vector<string> &tokens, vector<vector<string>> &num_pipes, vector<vector<string>> &cmds)
{
    vector<string>::iterator pre_end = tokens.begin();
    vector<string> tokens_after_num_pipe;
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if (std::regex_match((*it), std::regex("\\|\\d+")))
        {
            // cout << *it << endl;
            // int n = stoi((*it).erase(0, 1));
            // cout << n << endl;
            if (pre_end != tokens.begin())
                pre_end += 1;
            vector<string> temp(pre_end, it);
            num_pipes.push_back(temp);
            pre_end = it;
        }
        else if (it == tokens.end() - 1)
        {
            if (std::regex_match((*pre_end), std::regex("\\|\\d+")))
                pre_end += 1;
            vector<string> temp(pre_end, tokens.end());
            split_by_pipe(temp, cmds);
        }
    }
    cout << "num_pipe" << endl;
    for (int i = 0; i < num_pipes.size(); i++)
    {
        for (int j = 0; j < num_pipes[i].size(); j++)
        {
            cout << "<" << num_pipes[i][j] << "> ";
        }
        cout << endl;
    }
    cout << "cmds" << endl;
    for (int i = 0; i < cmds.size(); i++)
    {
        for (int j = 0; j < cmds[i].size(); j++)
        {
            cout << "<" << cmds[i][j] << "> ";
        }
        cout << endl;
    }
}
void parser(string &input)
{
    if (input.empty())
        return;
    vector<string> tokens;
    istringstream iss(input);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
    vector<vector<string>> num_pipes;
    vector<vector<string>> cmds;
    split_by_number_pipe(tokens, num_pipes, cmds);
    switch (is_built_in_command(tokens))
    {
    case NOT_BUILT_IN:
        exe_bin(cmds);
        cout << "out_loop(parent):" << getpid() << endl;
        break;
    case SETENV:
        setenv(tokens[1].c_str(), tokens[2].c_str(), true);
        break;
    case PRINTENV:
        print_env(tokens[1].c_str());
        break;
    case EXIT:
        exit(0);
        break;
    }
}