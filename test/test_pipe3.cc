#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESS_NUM 10
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

int main(int argc, char *argv[])
{
    int pids[PROCESS_NUM];
    int pipes[PROCESS_NUM + 1][2];
    int i;
    for (i = 0; i < PROCESS_NUM + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error with creating pipe\n");
            return 1;
        }
    }

    for (i = 0; i < PROCESS_NUM; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            printf("Error with creating process\n");
            return 2;
        }
        if (pids[i] == 0)
        {
            // Child process
            int j;
            for (j = 0; j < PROCESS_NUM + 1; j++)
            {
                if (i != j)
                {
                    close(pipes[j][0]);
                }
                if (i + 1 != j)
                {
                    close(pipes[j][1]);
                }
            }
            int x;
            if (read(pipes[i][0], &x, sizeof(int)) == -1)
            {
                printf("Error at reading\n");
                return 3;
            }
            printf("(%d) Got %d\n", i, x);
            x++;
            if (write(pipes[i + 1][1], &x, sizeof(int)) == -1)
            {
                printf("Error at writing\n");
                return 4;
            }
            printf("(%d) Sent %d\n", i, x);
            close(pipes[i][0]);
            close(pipes[i + 1][1]);
            return 0;
        }
    }

    // Main process
    int j;
    for (j = 0; j < PROCESS_NUM + 1; j++)
    {
        if (j != PROCESS_NUM)
        {
            close(pipes[j][0]);
        }
        if (j != 0)
        {
            close(pipes[j][1]);
        }
    }
    int y = 5;
    printf("Main process sent %d\n", y);
    if (write(pipes[0][1], &y, sizeof(int)) == -1)
    {
        printf("Error at writing\n");
        return 4;
    }
    if (read(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1)
    {
        printf("Error at reading\n");
        return 3;
    }
    printf("The final result is %d\n", y);
    close(pipes[0][1]);
    close(pipes[PROCESS_NUM][0]);

    for (i = 0; i < PROCESS_NUM; i++)
    {
        wait(NULL);
    }
    return 0;
}