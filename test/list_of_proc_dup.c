#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
void main()
{
    int i;
    pid_t pid;
    for (i = 0; i < 3; i++)
    {
        pid = fork();
        // This printf statement is for debugging purposes
        // getppid(): gets the parent process-id
        // getpid(): get child process-id

        printf("[%d] [%d] i=%d\n", getppid(), getpid(), i);
    }
    printf("[%d] [%d] hi\n", getppid(), getpid());
    return;
}