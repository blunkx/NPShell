#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

// for example, let's "ls"
void ls(const char *dir)
{
    // first we fork the process
    pid_t pid, status;
    pid = fork();
    if (pid == 0)
    {
        std::cout << "child:" << pid << std::endl;
        /* pid == 0: this is the child process. now let's load the
           "ls" program into this process and run it */
        const char executable[] = "/bin/ls";
        execl(executable, executable, dir, NULL);
        // load it. there are more exec__ functions, try 'man 3 exec'
        // execl takes the arguments as parameters. execv takes them as an array
        // this is execl though, so:
        //      exec         argv[0]  argv[1] end
    }
    else
    {
        /* exec does not return unless the program couldn't be started.
           when the child process stops, the waitpid() above will return.
        */
        //  pid != 0: this is the parent process (i.e. our process)
        waitpid(pid, &status, 0); // wait for the child to exit
        std::cout << "parent:" << pid << std::endl;
    }
    return;
}

int main()
{

    std::cout << "ls'ing /" << std::endl;
    ls("/");
    // std::cout << "returned: " <<  << std::endl;

    return 0;
}