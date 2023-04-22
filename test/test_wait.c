#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
pid_t fd1 = 0;
pid_t fd2 = 0;
int stat = 0;
int wd1 = 0;
int wd2 = 0;
void sigchld_handler(int sig)
{
    // represent if this function is called.
    printf("father call the SIGCHLD signal handler. \n");
    wd1 = waitpid(fd1, &stat, 0);
    wd2 = waitpid(fd2, &stat, 0);
    if (wd1 == fd1)
    {
        // childl finish
        printf("child process has finished. Nn");
    }
    if (wd2 == fd2)
    {
        // child2 finish
        printf("child2 process has finished. Nn");
    }
}
int main()
{
    signal(SIGCHLD, sigchld_handler);
    fd1 = fork();
    if (fd1 == -1)
    {
        exit(1);
    }
    else if (fd1 == 0)
    {
        // childl code
        printf("child process is running, pid = %d. \n", getpid());
    }
    else
    {
        fd2 = fork();
        switch (fd2)
        {
        case -1:
            exit(1);
            break;
        case 0: // child2 code
            printf("child process is running, pid = %d. \n", getpid());
            break;
        default: // father code
            pause();
            printf("father process runs again.\n");
            break;
        }
    }

    return 0;
}
