#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int stdout_cpy = dup(1); // Mostly irrelevant

    printf("1. stdout working\n");

    int pipe1[2];
    pipe(pipe1);

    int pid = fork();

    if (pid == 0)
    {
        // child at work
        dup2(pipe1[1], 1); // stdout out to write end of pipe
        close(pipe1[0]);   // Close both ends of the pipe!
        close(pipe1[1]);   // Close both ends of the pipe!
        printf("2. This should be read by parent from stdin and be printed to the parent's stdout\n");
        exit(0);
    }

    /* Parent at work */
    dup2(pipe1[0], 0); // stdin from from read end of pipe
    close(pipe1[0]);   // Close both ends of the pipe!
    close(pipe1[1]);   // Close both ends of the pipe!

    // Read data written on pipe by child, and write to stdout
    char buffer[512];
    int nbytes = read(0, buffer, sizeof(buffer));
    if (nbytes > 0)
        write(1, buffer, nbytes);

    /* 2. should print in parent's stdout... */
    int status;
    int corpse;
    while ((corpse = wait(&status)) > 0)
        printf("%d: child %d exited with status 0x%.4X\n", (int)getpid(), corpse, status);

    printf("3. This should print too\n");

    dup2(stdout_cpy, 1); // Mostly irrelevant
    // close(pipe1[0]);                  // Long since closed
    close(stdout_cpy); // No longer needed (closed on exit anyway)

    printf("4. stdout redirected, done\n");

    return 0;
}