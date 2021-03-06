#include <iostream>
#include <string>

#include "Run.h"

void printUser();

int main()
{
    Run run;

    for (;;)
    {
        printUser();

        std::string userInput;
        std::getline(std::cin, userInput);
        
        // b will return false if start failed
        bool b = run.start(userInput);

        if (!b)
            return 0;
    }

    return 0;
}

void printUser()
{
    char hostname[HOST_NAME_MAX];
    int status = gethostname(hostname, HOST_NAME_MAX);
    if (status != 0)
    {
        perror("gethostname failed");
        exit(errno);
    }

    struct passwd *pws;
    pws = getpwuid(geteuid());
    if (pws == NULL)
    {
        perror("getid failed");
        exit(errno);
    }
    std::string login = pws->pw_name;

    std::cout << login << "@" << hostname << "$ ";
}
