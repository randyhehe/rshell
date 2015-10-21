#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

std::vector<std::string> splitString(std::string str);

int main()
{
    std::string userInput;
    
    for(;;)
    {
        std::getline(std::cin, userInput);
        if(userInput == "exit")
            exit(0);

        std::vector<std::string> vecWords = splitString(userInput);
    }

    return 0;
}

std::vector<std::string> splitString(std::string str)
{
    std::vector<std::string> vectorWords;

    std::istringstream iSS(str);
    do
    {
        std::string singleWord;
        iSS >> singleWord;
        vectorWords.push_back(singleWord);
    } while(iSS);

    return vectorWords;
}
