// pipe_simple.cpp
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{

    int p1[2];
    int status;
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
        execlp("cat", "cat", NULL);
        exit(0);
    }
    else
    {
        // waitpid(0, &status, 0);
        /*parent process*/
        std::cout << "this is parent process!" << std::endl;
        dup2(p1[1], STDOUT_FILENO);
        close(p1[0]);
        close(p1[1]);
        std::cout << "can u hear me?" << std::flush;
    }
}