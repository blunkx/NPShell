#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
// for example, let's "ls"
char **vector_to_c_str_arr(vector<string> cmd)
{
    char **arr = (char **)malloc((cmd.size() + 1) * sizeof(char *));
    int i = 0;
    arr[cmd.size()] = NULL;
    for (i = 0; i < cmd.size(); i++)
    {
        arr[i] = strdup((cmd[i]).c_str());
    }

    for (i = 0; i < cmd.size(); i++)
    {
        cout << "it|" << arr[i] << "|\t";
    }
    cout << endl;
    return arr;
}
void ls(const char *dir)
{
    setenv("PATH", "bin:.", true);
    // first we fork the process
    pid_t pid, status;
    pid = fork();
    if (pid == 0)
    {
        std::cout << "child:" << pid << std::endl;
        /* pid == 0: this is the child process. now let's load the
           "ls" program into this process and run it */
        // const char executable[] = "ls";
        // if (execlp("ls", "include", dir, NULL) == -1)
        // {
        //     std::cout << "err" << std::endl;
        // }
        vector<string> test({"ls", "-l", "test/"});
        char **executable = vector_to_c_str_arr(test);
        if (execvp(executable[0], executable) == -1)
        {
            std::cout << "err" << std::endl;
        }
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
    vector<vector<string>> cmds;
    for (int i = 0; i < 10; i++)
    {
        // vector<string> temp({"cat", "test.html"});
        vector<string> temp({"ls", "-l", "test/"});
        cmds.push_back(temp);
    }

    int status;
    for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    {
        char **args = (char **)malloc((*it).size() * sizeof(char *));
        int i = 0;
        args[(*it).size()] = NULL;
        for (i = 0; i < (*it).size(); i++)
        {
            args[i] = strdup(((*it)[i]).c_str());
        }

        pid_t pid;
        pid = fork();
        if (pid == -1)
        {
        }
        else if (pid == 0)
        {

            char **args = (char **)malloc((*it).size() * sizeof(char *));
            int i = 0;
            args[(*it).size()] = NULL;
            for (i = 0; i < (*it).size(); i++)
            {
                args[i] = strdup(((*it)[i]).c_str());
            }
            for (i = 0; i < (*it).size(); i++)
            {
                cout << "it|" << args[i] << "|\t";
            }
            cout << endl;
            // print_env("PATH");
            // vector<string> temp({"cat", "test.html"});
            // vector<string> temp({"ls", "-l", "test/"});
            // args = vector_to_c_str_arr(*it);
            // cout << args[0] << endl;
            // if (execlp("ls", "include", "/", NULL) == -1)
            // {
            //     fprintf(stderr, "Unknown command: [%s].\n", (*it)[0].c_str());
            //     exit(EXIT_FAILURE);
            // }
            if (execvp(args[0], args) == -1)
            {
                fprintf(stderr, "Unknown command: [%s].\n", args[0]);
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
        else
        {
            waitpid(pid, &status, 0); // wait for the child to exit
        }
    }
    // for (vector<vector<string>>::iterator it = cmds.begin(); it != cmds.end(); it++)
    //     waitpid(0, &status, 0);
    // ls("/");
    return 0;
}