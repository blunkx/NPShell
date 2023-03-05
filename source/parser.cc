#include "parser.h"

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
void print_cmds(vector<command> cmds)
{
    for (int i = 0; i < cmds.size(); i++)
    {
        for (int j = 0; j < cmds[i].cmd.size(); j++)
        {
            cout << "{" << cmds[i].cmd[j] << "} ";
        }
        cout << cmds[i].which_type();
        if (cmds[i].pipe_type == NUM_PIPE || cmds[i].pipe_type == ERR_NUM_PIPE)
            cout << " " << cmds[i].pipe_num << endl;
        else
            cout << endl;
    }
}

inline void reduce_num_pipes(vector<command> &number_pipes)
{
    for (int i = 0; i < number_pipes.size(); i++)
    {
        if (number_pipes[i].pipe_num > 0)
            number_pipes[i].pipe_num--;
    }
}

void split_by_pipe(vector<string> &tokens, vector<command> &cmds, vector<command> &number_pipes)
{
    vector<string>::iterator pre_end = tokens.begin();
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        command temp;
        if (*it == "|")
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = PIPE;
            vector<string> temp_cmd(pre_end, it);
            temp.cmd = temp_cmd;
            cmds.push_back(temp);
            pre_end = it + 1;
        }
        else if (*it == "!")
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = ERR_PIPE;
            vector<string> temp_cmd(pre_end, it);
            temp.cmd = temp_cmd;
            cmds.push_back(temp);
            pre_end = it + 1;
        }
        else if (*it == ">")
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = PIPE;
            vector<string> temp_cmd(pre_end, it);
            temp.cmd = temp_cmd;
            cmds.push_back(temp);

            command temp_frd;
            temp_frd.pipe_type = F_RED_PIPE;
            vector<string> fname(it + 1, it + 2);
            temp_frd.cmd = fname;
            cmds.push_back(temp_frd);
            it++;
            pre_end = it + 1;
        }
        else if (std::regex_match((*it), std::regex("\\|\\d+")))
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = NUM_PIPE;
            vector<string> temp_cmd(pre_end, it);
            temp.cmd = temp_cmd;
            temp.pipe_num = stoi((*it).erase(0, 1));
            number_pipes.push_back(temp);
            pre_end = it + 1;
        }
        else if (std::regex_match((*it), std::regex("\\!\\d+")))
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = ERR_NUM_PIPE;
            vector<string> temp_cmd(pre_end, it);
            temp.cmd = temp_cmd;
            temp.pipe_num = stoi((*it).erase(0, 1));
            number_pipes.push_back(temp);
            pre_end = it + 1;
        }
        else if (it == tokens.end() - 1)
        {
            reduce_num_pipes(number_pipes);
            temp.pipe_type = NO_PIPE;
            vector<string> temp_cmd(pre_end, tokens.end());
            temp.cmd = temp_cmd;
            cmds.push_back(temp);
        }
    }
    cout << "number pipe:" << endl;
    print_cmds(number_pipes);
    cout << "pipe:" << endl;
    print_cmds(cmds);
    cout << endl;
}

void parser(string &input, vector<command> &number_pipes)
{
    if (input.empty())
        return;
    vector<string> tokens;
    istringstream iss(input);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
    vector<command> cmds;
    split_by_pipe(tokens, cmds, number_pipes);
    switch (is_built_in_command(tokens))
    {
    case NOT_BUILT_IN:
        exe_bin(cmds, number_pipes);
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

    number_pipes.erase(
        remove_if(
            number_pipes.begin(),
            number_pipes.end(),
            [](command const &p)
            { return p.pipe_num == 0; }),
        number_pipes.end());
}