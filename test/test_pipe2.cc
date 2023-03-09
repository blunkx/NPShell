#include <iostream>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
#define PROCESS_NUM 10
int main(int argc, char *argv[])
{
    setenv("PATH", "bin:.", true);
    int pids[PROCESS_NUM];
    int pipes[PROCESS_NUM + 1][2];
    int init_pipe;
    int stdout_copy = dup(STDOUT_FILENO);
    int i;
    int status;
    for (i = 0; i < PROCESS_NUM; i++)
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
            for (j = 0; j < PROCESS_NUM; j++)
            {
                if (i != j)
                {
                    close(pipes[j][0]);
                }
                else
                    cout << j << " R" << endl;

                if (i + 1 != j)
                {
                    close(pipes[j][1]);
                }
                else
                    cout << j << " W" << endl;
            }
            cout << endl;
            if (i == PROCESS_NUM - 1)
            {
                // std::cout << "exe" << std::endl;
                dup2(pipes[i][0], STDIN_FILENO);
                close(pipes[i][0]);
                dup2(stdout_copy, STDOUT_FILENO);
                std::string arg1("cat");
                char *arg[2] = {strdup(arg1.c_str()), NULL};
                if (execvp(arg[0], arg) == -1)
                {
                    perror("Error: ");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // std::cout << "exe" << std::endl;

                if (i == 0)
                {
                    dup2(pipes[i][0], STDIN_FILENO);
                    dup2(pipes[i + 1][1], STDOUT_FILENO);
                    close(pipes[i][0]);
                    close(pipes[i + 1][1]);
                    // std::string arg1("cat");
                    // std::string arg2("test.html");
                    std::string arg1("ls");
                    std::string arg2("-l");
                    char *arg[3] = {strdup(arg1.c_str()), strdup(arg2.c_str()), NULL};
                    if (execvp(arg[0], arg) == -1)
                    {
                        perror("Error: ");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    dup2(pipes[i][0], STDIN_FILENO);
                    dup2(pipes[i + 1][1], STDOUT_FILENO);
                    close(pipes[i][0]);
                    close(pipes[i + 1][1]);
                    std::string arg1("cat");
                    char *arg[2] = {strdup(arg1.c_str()), NULL};
                    if (execvp(arg[0], arg) == -1)
                    {
                        perror("Error: ");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        // else
        // {
        //     waitpid(0, &status, 0);
        // }
    }
    // Main process
    int j;
    for (j = 0; j < PROCESS_NUM; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }
    for (i = 0; i < PROCESS_NUM; i++)
    {
        wait(NULL);
    }
    return 0;
}