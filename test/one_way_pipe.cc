// pipe_simple.cpp
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    setenv("PATH", "bin:.", true);
    int p1[2];
    int status;
    int stdout_copy = dup(STDOUT_FILENO);
    if (pipe(p1) < 0)
        std::cout << "pipe1 create error" << std::endl;
    signal(SIGCHLD, SIG_IGN);
    if (fork() == 0)
    {
        /*child process*/
        std::cout << "this is child process" << std::endl;
        dup2(p1[0], STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
        close(p1[0]);
        close(p1[1]);
        dup2(stdout_copy, STDOUT_FILENO);
        execlp("cat", "cat", NULL);
        // exit(0);
    }
    else
    {
        /*parent process*/
        close(p1[0]);
        std::cout << "this is parent process!" << std::endl;
        dup2(p1[1], STDOUT_FILENO);
        std::cout << "can u hear me?" << std::endl;
        close(p1[1]);
        // waitpid(0, &status, 0);
    }
}