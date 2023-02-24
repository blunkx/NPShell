#include "command.h"

void init_env()
{
    setenv("PATH", "bin:.", true);
}
void print_env(const char *const para)
{
    char *cur_env;
    cur_env = getenv(para);
    if (cur_env != NULL)
        cout << cur_env << endl;
}

char **vector_to_c_str_arr(vector<string> cmd)
{
    char **arr = (char **)malloc((cmd.size() + 1) * sizeof(char *));
    for (int i = 0; i < cmd.size(); i++)
    {
        arr[i] = strdup((cmd[i]).c_str());
    }
    arr[cmd.size()] = NULL;
    return arr;
}

void exe_bin(vector<vector<string>> cmds)
{
    int status;
    for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    {
        pid_t pid;
        pid = fork();
        if (pid == -1)
        {
            cerr << "fork error!\n";
        }
        else if (pid == 0)
        {
            char **args = vector_to_c_str_arr(*it);
            if (execvp(args[0], args) == -1)
            {
                // Q will memory space recycle>?
                perror("Error: ");
                cerr << "Unknown command: {" << args[0] << "}.\n";
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            waitpid(pid, &status, 0); // wait for the child to exit
        }
    }
    // for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    //     waitpid(0, &status, 0);
    return;
}
