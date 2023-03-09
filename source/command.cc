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
            cout << " " << cmds[i].pipe_num;
        if (cmds[i].is_exe)
            cout << " exed";
        if (cmds[i].is_piped)
            cout << " piped";

        // if (cmds[i].is_first_cmd)
        //     cout << " 1st" << endl;
        // else
        //     cout << endl;
        cout << endl;
    }
}

inline void reduce_num_pipes(vector<command> &number_pipes, int last)
{
    for (int i = 0; i < last; i++)
    {
        if (number_pipes[i].pipe_num > 0)
            number_pipes[i].pipe_num--;
    }
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

void exe_command(int stdout_copy, vector<command> &cmds, int i, bool stop_pipe, int temp_fd[])
{
    if (stop_pipe)
    {
        dup2(temp_fd[0], STDIN_FILENO);
    }
    else
    {
        dup2(cmds[i - 1].fd[0], STDIN_FILENO);
        close(cmds[i - 1].fd[0]);
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    close(cmds[i].fd[0]);
    close(cmds[i].fd[1]);
    dup2(stdout_copy, STDOUT_FILENO);
    char **args = vector_to_c_str_arr(cmds[i].cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_pipe(int stdout_copy, vector<command> &cmds, int i, bool stop_pipe, int temp_fd[])
{
    if (stop_pipe)
    {
        dup2(temp_fd[0], STDIN_FILENO);
    }
    else
    {
        dup2(cmds[i - 1].fd[0], STDIN_FILENO);
        close(cmds[i - 1].fd[0]);
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    if (i == 0)
    {
        close(cmds[0].fd[0]);
    }
    dup2(cmds[i].fd[1], STDOUT_FILENO); // stdout refer to p2[1]
    close(cmds[i].fd[1]);
    char **args = vector_to_c_str_arr(cmds[i].cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_err_pipe(int stdout_copy, vector<command> &cmds, int i, bool stop_pipe, int temp_fd[])
{
    if (stop_pipe)
    {
        dup2(temp_fd[0], STDIN_FILENO);
    }
    else
    {
        dup2(cmds[i - 1].fd[0], STDIN_FILENO);
        close(cmds[i - 1].fd[0]);
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    if (i == 0)
    {
        close(cmds[0].fd[0]);
    }
    dup2(cmds[i].fd[1], STDERR_FILENO); // stderr refer to p2[1]
    dup2(cmds[i].fd[1], STDOUT_FILENO); // stdout refer to p2[1]
    close(cmds[i].fd[1]);
    char **args = vector_to_c_str_arr(cmds[i].cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_f_red(int stdout_copy, vector<command> &cmds, int i, int temp_fd[])
{
    close(temp_fd[0]);
    close(temp_fd[1]);
    close(cmds[i].fd[0]);
    close(cmds[i].fd[1]);
    if (i > 0)
    {
        dup2(cmds[i - 1].fd[0], STDIN_FILENO); // stdin refer to p1[0]
        close(cmds[i - 1].fd[0]);
    }
    else if (i == 0)
        cerr << "file redirection error!\n";
    int fd = open(cmds[i].cmd[0].c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    if (execlp("cat", "cat", NULL) == -1)
    {
        cerr << "file redirection failed!\n";
        exit(EXIT_FAILURE);
    }
}

void exe_num_pipe(int stdout_copy, vector<command> &cmds, int i, bool stop_pipe, int temp_fd[])
{
    cout << "xxx" << endl;
    if (stop_pipe)
    {
        cout << "x222" << endl;
        dup2(temp_fd[0], STDIN_FILENO);
    }
    else
    {
        cout << "111" << endl;
        dup2(cmds[i - 1].fd[0], STDIN_FILENO);
        close(cmds[i - 1].fd[0]);
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    dup2(cmds[i].fd[1], STDOUT_FILENO); // stdout refer to p2[1]
    close(cmds[i].fd[1]);
    char **args = vector_to_c_str_arr(cmds[i].cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_err_num_pipe(int stdout_copy, vector<command> &cmds, int i, bool stop_pipe, int temp_fd[])
{
    if (stop_pipe)
    {
        dup2(temp_fd[0], STDIN_FILENO);
    }
    else
    {
        dup2(cmds[i - 1].fd[0], STDIN_FILENO);
        close(cmds[i - 1].fd[0]);
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    if (i == 0)
    {
        close(cmds[0].fd[0]);
    }
    dup2(cmds[i].fd[1], STDERR_FILENO); // stdout refer to p2[1]
    dup2(cmds[i].fd[1], STDOUT_FILENO); // stdout refer to p2[1]
    close(cmds[i].fd[1]);
    char **args = vector_to_c_str_arr(cmds[i].cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_bin(vector<command> &cmds)
{
    int status;
    // int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    string temp_output;
    for (int i = 0; i < cmds.size(); i++)
    {
        if (!cmds[i].is_exe)
        {
            if (pipe(cmds[i].fd) == -1)
            {
                cerr << "pipe error\n";
                exit(EXIT_FAILURE);
            }
        }
    }
    // pipe(temp_fd);
    // for (int i = first_index - 1; i >= 1; i--)
    // {
    //     pid_t pid;
    //     pid = fork();
    //     if (pid == 0)
    //     {
    //         dup2(cmds[i].fd[0], STDIN_FILENO);
    //         close(cmds[i].fd[0]);
    //         close(cmds[i].fd[1]);
    //         dup2(cmds[0].fd[1], STDOUT_FILENO);
    //         close(cmds[0].fd[1]);
    //         if (execlp("cat", "cat", NULL) == -1)
    //         {
    //             cerr << "file redirection failed!\n";
    //             exit(EXIT_FAILURE);
    //         }
    //     }
    // }

    bool stop_pipe = true;
    int temp_fd[2];
    pipe(temp_fd);
    for (int i = 0; i < cmds.size(); i++)
    {
        reduce_num_pipes(cmds, i);
        for (int j = 0; j < i; j++)
        {
            if (cmds[j].pipe_type == NUM_PIPE || cmds[j].pipe_type == ERR_NUM_PIPE)
            {
                if (!cmds[j].is_piped && cmds[j].pipe_num == 0)
                {
                    pid_t pid;
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
                        dup2(cmds[j].fd[0], STDIN_FILENO);
                        close(cmds[j].fd[0]);
                        close(cmds[j].fd[1]);
                        dup2(temp_fd[1], STDOUT_FILENO);
                        close(temp_fd[0]);
                        close(temp_fd[1]);
                        if (execlp("cat", "cat", NULL) == -1)
                        {
                            cerr << "cat error\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        cmds[j].is_piped = true;
                        close(cmds[j].fd[0]);
                        close(cmds[j].fd[1]);
                        wait(NULL);
                    }
                }
            }
        }
        cout << i << "th pipe" << endl;
        print_cmds(cmds);
        cout << endl
             << endl;
        pid_t pid;
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
            //  child process
            for (int j = 0; j < cmds.size(); j++)
            {
                if (i - 1 != j)
                {
                    close(cmds[j].fd[0]);
                }
                // else
                //     cout << j << " R" << endl;
                if (i != j)
                {
                    close(cmds[j].fd[1]);
                }
                // else
                //     cout << j << " W" << endl;
            }
            // cout << endl;
            if (cmds[i].pipe_type == NO_PIPE)
            {
                exe_command(stdout_copy, cmds, i, stop_pipe, temp_fd);
            }
            else if (cmds[i].pipe_type == PIPE)
            {
                exe_pipe(stdout_copy, cmds, i, stop_pipe, temp_fd);
            }
            else if (cmds[i].pipe_type == ERR_PIPE)
            {
                exe_err_pipe(stdout_copy, cmds, i, stop_pipe, temp_fd);
            }
            else if (cmds[i].pipe_type == F_RED_PIPE)
            {
                exe_f_red(stdout_copy, cmds, i, temp_fd);
            }
            else if (cmds[i].pipe_type == NUM_PIPE)
            {
                if (!cmds[i].is_exe)
                    exe_num_pipe(stdout_copy, cmds, i, stop_pipe, temp_fd);
                else
                    exit(EXIT_SUCCESS);
            }
            else if (cmds[i].pipe_type == ERR_NUM_PIPE)
            {
                if (!cmds[i].is_exe)
                    exe_err_num_pipe(stdout_copy, cmds, i, stop_pipe, temp_fd);
                else
                    exit(EXIT_SUCCESS);
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // parent process
            // cout << stop_pipe << endl;
            if (cmds[i].pipe_type == NUM_PIPE || cmds[i].pipe_type == ERR_NUM_PIPE)
            {
                stop_pipe = true;
            }
            else
            {
                stop_pipe = false;
                cmds[i].is_piped = true;
            }
            cmds[i].is_exe = true;

            // close(temp_fd[1]);
            //  waitpid(pid, &status, 0);
        }
    }
    close(temp_fd[0]);
    close(temp_fd[1]);
    for (int i = 0; i < cmds.size(); i++)
    {
        switch (cmds[i].pipe_type)
        {
        case NO_PIPE:
        case PIPE:
        case ERR_PIPE:
        case F_RED_PIPE:
            close(cmds[i].fd[0]);
            close(cmds[i].fd[1]);
            break;
        case NUM_PIPE:
        case ERR_NUM_PIPE:
            if (cmds[i].pipe_num == 0)
            {
                close(cmds[i].fd[0]);
                close(cmds[i].fd[1]);
            }
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < cmds.size(); i++)
    {
        waitpid(0, &status, 0);
    }
    return;
}
