
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "err.h"
#define DEPTH 4
// Set DEPTH to desired value
int main()
{
    pid_t pid;
    int i;

    for (i = 1; i <= DEPTH; i++)
    {

        pid = fork(); // Fork

        if (pid)
        {
            break; // Don't give the parent a chance to fork again
        }
        std::cout << "Child #" << getpid() << std::endl; // Child can keep going and fork once
    }
    wait(NULL); // Don't let a parent ending first end the tree below
    std::cout << "My process id = " << getpid() << std::endl;
    return 0;
}