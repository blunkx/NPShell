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
        // arr[i] = (i == cmd.size() - 1) ? strdup((cmd[i]).c_str()) : NULL;
        arr[i] = strdup((cmd[i]).c_str());
    }
    arr[cmd.size()] = NULL;
    return arr;
}

void exe_bin(vector<vector<string>> cmds)
{
    int status;
    int p1[2];
    int p2[2];
    char pipe_buff[100000] = {0};
    // int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    {
        pid_t pid;
        signal(SIGCHLD, SIG_IGN); // wait all child process close
        if (pipe(p1) < 0)
            cerr << "pipe error\n";
        if (pipe(p2) < 0)
            cerr << "pipe error\n";
        pid = fork();
        if (pid == -1)
        {
            cerr << "fork error!\n";
        }
        else if (pid == 0)
        {
            if (it + 1 == cmds.end())
            {
                dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                dup2(stdout_copy, STDOUT_FILENO);

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
                dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                dup2(p2[1], STDOUT_FILENO);

                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                char **args = vector_to_c_str_arr(*it);
                if (execvp(args[0], args) == -1)
                {
                    // Q will memory space recycle>?
                    perror("Error: ");
                    cerr << "Unknown command: {" << args[0] << "}.\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            dup2(p1[1], STDOUT_FILENO);
            close(p1[0]);
            close(p1[1]);
            cout << pipe_buff << flush;
            dup2(stdout_copy, STDOUT_FILENO); /*p1 write end isn't used anymore, send EOF*/
            if (it + 1 != cmds.end())
            {
                memset(pipe_buff, 0, sizeof(pipe_buff));
                read(p2[0], pipe_buff, sizeof(pipe_buff));
            }
            close(p2[0]); // p2 close here
            close(p2[1]);
            waitpid(pid, &status, 0); // wait for the child to exit
        }
    }
    // for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    //     waitpid(0, &status, 0);
    return;
}
