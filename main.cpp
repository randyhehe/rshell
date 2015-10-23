#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

std::vector<std::string> splitString(std::string str);

int main(int argc, char** argv)
{
    char* args[argc];
    for(int i = 1; i < argc; i++)
    {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = NULL;

    pid_t processID;
    int status;

    processID = fork();
    
    if(processID < 0)
    {
        perror("fork failed");
        exit(1);
    }

    else if(processID == 0)
    {
        execvp(args[0], args);

        for(int i = 0; i < argc - 1; i++)
        {
            std::cout << args[i] << std::endl;
        }

        perror("execvp failed");
    }
    else
    {
        if((processID = wait(&status)) < 0)
        {
            perror("wait");
            exit(1);
        }

        std::cout << "hello" << std::endl;
    }

    return 0;
}

// Splits a string of sentences to a vector of words
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
