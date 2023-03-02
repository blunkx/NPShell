#include "command.h"

string command::which_type()
{
    switch (pipe_type)
    {
    case NO_PIPE:
        return "NO_PIPE";
    case PIPE:
        return "PIPE";
    case ERR_PIPE:
        return "ERR_PIP";
    case F_RED_PIPE:
        return "F_RED_PIPE";
    case NUM_PIPE:
        return "NUM_PIPE";
    case ERR_NUM_PIPE:
        return "ERR_NUM_PIPE";
    default:
        return "error";
    }
}

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

void exe_bin(vector<command> cmds)
{
    int status;
    int p1[2];
    int p2[2];
    char pipe_buff[100000] = {0};
    // int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    for (int i = 0; i < cmds.size(); i++)
    {
        // if (cmds[i].pipe_type == NUM_PIPE || cmds[i].pipe_type == ERR_NUM_PIPE)
        //     continue;
        pid_t pid;
        if (pipe(p1) < 0)
            cerr << "pipe error\n";
        if (pipe(p2) < 0)
            cerr << "pipe error\n";

        pid = fork();
        if (pid == -1)
        {
            cerr << "fork error!\n";
            while (true)
            {
                if (waitpid(pid, &status, WNOHANG) == pid)
                    break;
            }
            pid = fork();
        }
        else if (pid == 0)
        {
            if (cmds[i].pipe_type == NO_PIPE)
            {
                dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/

                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                dup2(stdout_copy, STDOUT_FILENO);
                char **args = vector_to_c_str_arr(cmds[i].cmd);
                if (execvp(args[0], args) == -1)
                {
                    perror("Error: ");
                    cerr << "Unknown command: {" << args[0] << "}.\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (cmds[i].pipe_type == F_RED_PIPE)
            {
                int fd = open(cmds[i].cmd[0].c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(fd, STDOUT_FILENO);
                cout << pipe_buff << flush;
                if (i < cmds.size() - 1)
                {
                    memset(pipe_buff, 0, sizeof(pipe_buff));
                    read(p2[0], pipe_buff, sizeof(pipe_buff));
                }
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                close(fd);
                dup2(stdout_copy, STDOUT_FILENO);
                // dup2(stdin_copy, STDIN_FILENO);
                exit(0);
            }
            else if (cmds[i].pipe_type == PIPE)
            {
                dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                dup2(p2[1], STDOUT_FILENO);
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                char **args = vector_to_c_str_arr(cmds[i].cmd);
                if (execvp(args[0], args) == -1)
                {
                    perror("Error: ");
                    cerr << "Unknown command: {" << args[0] << "}.\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (cmds[i].pipe_type == ERR_PIPE)
            {
                dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                dup2(p2[1], STDERR_FILENO);
                dup2(p2[1], STDOUT_FILENO);
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                char **args = vector_to_c_str_arr(cmds[i].cmd);
                if (execvp(args[0], args) == -1)
                {
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
                exit(0);
            }
        }
        else
        {
            dup2(p1[1], STDOUT_FILENO);
            close(p1[0]);
            close(p1[1]);
            cout << pipe_buff << flush;
            dup2(stdout_copy, STDOUT_FILENO); /*p1 write end isn't used anymore, send EOF*/
            close(p2[1]);
            waitpid(pid, &status, 0); // wait for the child to exit
            if (i < cmds.size() - 1)
            {
                memset(pipe_buff, 0, sizeof(pipe_buff));
                read(p2[0], pipe_buff, sizeof(pipe_buff));
            }
            close(p2[0]); // p2 close here
        }
    }
    // for (int i = 0; i < cmds.size(); i++)
    //     waitpid(0, &status, 0);
    return;
}
