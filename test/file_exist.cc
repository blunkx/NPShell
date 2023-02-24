#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
using namespace std;

inline bool exists_test0(const std::string &name)
{
    ifstream f(name.c_str());
    return f.good();
}

inline bool exists_test1(const std::string &name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

inline bool exists_test2(const std::string &name)
{
    return (access(name.c_str(), F_OK) != -1);
}

inline bool exists_test3(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}
int main()
{
    setenv("PATH", "bin:.", true);
    std::cout << "ls'ing /" << std::endl;
    if (exists_test3("ls"))
    {
        cout << "exist" << endl;
    }
    else
    {
    }
    return 0;
}