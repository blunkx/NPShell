#include "parser.h"

// bool is_valid_command(const string &token)
// {

// }

BUILT_IN_COM_E is_built_in_command(const vector<string> tokens)
{
    /*
    setenv [var] [value]
    printenv [var]
    exit
    */
    if (tokens[0] == "setenv" && tokens.size() == 3)
        return SETENV;
    else if (tokens[0] == "printenv" && tokens.size() == 2)
        return PRINTENV;
    else if (tokens[0] == "exit" && tokens.size() == 1)
        return EXIT;
    else
        return NOT_BUILT_IN;
}
// bool is_command(const string &token)
// {
// }

void parser(string &input)
{
    if (input.empty())
        return;

    vector<string> tokens;
    // cout << input << endl;
    istringstream iss(input);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));

    switch (is_built_in_command(tokens))
    {
    case NOT_BUILT_IN:
        // pid_t pid;
        // int status;
        exe_bin(tokens);

        for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
        {
            // if token is command
            //      iter until next command or pipe
            // else
            //      discard and show in stderr
            //      dash(ex: -n) => arg for previous command(including unkhown) => discard it
            cout << *it << endl;

            // pid = fork();
            // if (pid == -1)
            // {
            // }
            // else if (pid == 0)
            // {
            //     const char executable[] = "/bin/ls";
            //     execl("/bin/ls", executable, "/", NULL);
            // }
        }
        // for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
        //     waitpid(0, &status, 0);
        // while (wait(&status) > 0)
        // {
        // }
        /* why not
        wait(NULL);
        waitpid(0, &status, 0); // wait for the last child to exit
        */
        // waitpid(pid, &status, 0); // wait for the last child to exit
        cout << "out_loop(parent):" << getpid() << endl;
        break;
    case SETENV:
        setenv(tokens[1].c_str(), tokens[2].c_str(), true);
        break;
    case PRINTENV:
        print_env(tokens[1].c_str());
        break;
    case EXIT:
        exit(0);
        break;
    }
}