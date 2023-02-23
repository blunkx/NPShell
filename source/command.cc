#include "command.h"

void init_env()
{
    setenv("PATH", "bin:.", true);
}
void print_env(const char *const para)
{
    char *cur_env;
    cur_env = getenv(para);
    if (cur_env != NULL)
        cout << cur_env << endl;
}

void exe_bin(vector<string> tokens)
{
    pid_t pid;
    int status;
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        // if token is command
        //      iter until next command or pipe
        // else
        //      discard and show in stderr
        //      dash(ex: -n) => arg for previous command(including unkhown) => discard it
        // cout << *it << endl;
        pid = fork();
        if (pid == -1)
        {
        }
        else if (pid == 0)
        {
            const char executable[] = "/bin/ls";
            execl("/bin/ls", executable, "/", NULL);
        }
    }
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
        waitpid(0, &status, 0);
    return;
}
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