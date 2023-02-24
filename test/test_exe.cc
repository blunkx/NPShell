// execvp_test.cpp
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
// void exe_bin()
// {
//     pid_t pid, status;
//     pid = fork();
//     if (pid == -1)
//     {
//     }
//     else if (pid == 0)
//     {
//         // std::cout << "par:" << getppid() << std::endl;
//         // std::cout << "child:" << getpid() << std::endl;
//         // cout << pid << endl;
//         /* pid == 0: this is the child process. now let's load the
//            "ls" program into this process and run it */
//         const char executable[] = "/bin/ls";
//         execl(executable, executable, "/", NULL);
//         // load it. there are more exec__ functions, try 'man 3 exec'
//         // execl takes the arguments as parameters. execv takes them as an array
//         // this is execl though, so:
//         //      exec         argv[0]  argv[1] end
//     }
//     else
//     {
//         // std::cout << "parent:" << getpid() << std::endl;
//         // cout << pid << endl;
//         /* exec does not return unless the program couldn't be started.
//            when the child process stops, the waitpid() above will return.
//         */
//         //  pid != 0: this is the parent process (i.e. our process)
//         // waitpid(pid, &status, 0); // wait for the child to exit
//         // std::cout << "parent:" << pid << std::endl;
//         // std::cout << "end_par" << std::endl;
//         return;
//     }
// }
int main()
{
    std::string arg1("cat");
    std::string arg2("test.html");
    char *arg[3] = {strdup(arg1.c_str()), strdup(arg2.c_str()), NULL};
    execvp(arg[0], arg); // works
    // if (execlp("ls", "include", "/", NULL) == -1)
    // {
    //     fprintf(stderr, "Unknown command: [%s].\n", (*it)[0].c_str());
    //     exit(EXIT_FAILURE);
    // }
}