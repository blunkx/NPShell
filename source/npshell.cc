#include "npshell.h"

int main(int argc, char *const argv[])
{
    init_env();
    exe_shell();
    return 0;
}
void exe_shell()
{
    string input;
    vector<command> cmds;
    while (true)
    {
        cout << "% ";
        if (!getline(cin, input).eof())
        {
            parser(input, cmds);
        }
        else
        {
            parser(input, cmds);
            break;
        }
    }
}
