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

// std::string qx(const std::vector<std::string> &args)
// {
//     int output[2];
//     pipe(output);

//     const pid_t pid = fork();
//     if (!pid)
//     {
//         // collect both stdout and stderr to the one pipe
//         close(output[0]);
//         dup2(output[1], STDOUT_FILENO);
//         dup2(output[1], STDERR_FILENO);
//         close(output[1]);

//         std::vector<char *> vc(args.size() + 1, NULL);
//         for (size_t i = 0; i < args.size(); ++i)
//         {
//             vc[i] = const_cast<char *>(args[i].c_str());
//         }

//         execvp(vc[0], &vc[0]);
//         // if execvp() fails, we do *not* want to call exit()
//         // since that can call exit handlers and flush buffers
//         // copied from the parent process
//         _exit(0);
//     }
//     close(output[1]);

//     std::string out;
//     const int buf_size = 4096;
//     char buffer[buf_size];
//     do
//     {
//         errno = 0;
//         const ssize_t r = read(output[0], buffer, buf_size);
//         if (r > 0)
//         {
//             out.append(buffer, r);
//         }
//     } while (errno == EAGAIN || errno == EINTR);
// }

void exe_command(int *p1, int *p2, int stdout_copy, command cmd)
{
    dup2(p1[0], STDIN_FILENO); // stdin refer to p1[0]
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    dup2(stdout_copy, STDOUT_FILENO);
    char **args = vector_to_c_str_arr(cmd.cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_pipe(int *p1, int *p2, int stdout_copy, command cmd)
{
    dup2(p1[0], STDIN_FILENO);  // stdin refer to p1[0]
    dup2(p2[1], STDOUT_FILENO); // stdout refer to p2[1]
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    char **args = vector_to_c_str_arr(cmd.cmd);
    if (execvp(args[0], args) == -1)
    {
        // perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_err_pipe(int *p1, int *p2, int stdout_copy, command cmd)
{
    dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
    dup2(p2[1], STDERR_FILENO);
    dup2(p2[1], STDOUT_FILENO);
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    char **args = vector_to_c_str_arr(cmd.cmd);
    if (execvp(args[0], args) == -1)
    {
        perror("Error: ");
        cerr << "Unknown command: {" << args[0] << "}.\n";
        exit(EXIT_FAILURE);
    }
}

void exe_f_red(int *p1, int *p2, int stdout_copy, command cmd, char pipe_buff[])
{
    int fd = open(cmd.cmd[0].c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // dup2(p1[0], STDIN_FILENO); need to read and pass to stdout
    dup2(fd, STDOUT_FILENO);
    cout << pipe_buff << flush;
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    close(fd);
    dup2(stdout_copy, STDOUT_FILENO); // recover stdout back
    exit(0);
}

void exe_bin(vector<command> cmds, vector<command> number_pipes)
{
    int status;
    int p1[2];
    int p2[2];
    char pipe_buff[100000] = {0};
    // int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);

    string num_pp_output;
    for (int i = number_pipes.size() - 1; i >= 0; i--)
    {
        if (number_pipes[i].pipe_num == 0)
        {
            cmds.insert(cmds.begin(), number_pipes[i]);
            if (number_pipes[i].pipe_type == NUM_PIPE)
                cmds[0].pipe_type = PIPE;
            else if (number_pipes[i].pipe_type == ERR_NUM_PIPE)
                cmds[0].pipe_type = ERR_PIPE;
        }
    }

    for (int i = 0; i < cmds.size(); i++)
    {
        if (cmds[i].pipe_type == NUM_PIPE || cmds[i].pipe_type == ERR_NUM_PIPE)
            continue;

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
            // child process
            if (cmds[i].pipe_type == NO_PIPE)
            {
                exe_command(p1, p2, stdout_copy, cmds[i]);
            }
            else if (cmds[i].pipe_type == PIPE)
            {
                exe_pipe(p1, p2, stdout_copy, cmds[i]);
            }
            else if (cmds[i].pipe_type == ERR_PIPE)
            {
                exe_err_pipe(p1, p2, stdout_copy, cmds[i]);
            }
            else if (cmds[i].pipe_type == F_RED_PIPE)
            {
                exe_f_red(p1, p2, stdout_copy, cmds[i], pipe_buff);
            }
            else if (cmds[i].pipe_type == NUM_PIPE)
            {
                dup2(p1[0], STDIN_FILENO);
                dup2(p2[1], STDOUT_FILENO);
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                exit(0);
            }
            else if (cmds[i].pipe_type == ERR_NUM_PIPE)
            {
                dup2(p1[0], STDIN_FILENO);
                dup2(p2[1], STDOUT_FILENO);
                close(p1[0]);
                close(p1[1]);
                close(p2[0]);
                close(p2[1]);
                exit(0);
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // parent process
            dup2(p1[1], STDOUT_FILENO); // stdout refer to p1[1]
            close(p1[0]);
            close(p1[1]);
            cout << pipe_buff << flush;
            dup2(stdout_copy, STDOUT_FILENO); // recover stdout back
            close(p2[1]);
            waitpid(pid, &status, 0); // wait for the child to exit
            if (i < cmds.size() - 1)
            {
                memset(pipe_buff, 0, sizeof(pipe_buff));
                read(p2[0], pipe_buff, sizeof(pipe_buff));
            }
            close(p2[0]);
        }
    }
    // for (int i = 0; i < cmds.size(); i++)
    //     waitpid(0, &status, 0);
    return;
}
