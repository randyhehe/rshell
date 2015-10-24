#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string>
#include <queue>

std::vector<std::vector<std::string> > splitCommands
    (std::string str, std::queue<std::string>& q);
std::vector<char *> stringToChar(std::vector<std::string>& s);
bool isConnector(std::string s);

int main(int argc, char** argv)
{
    std::string userInput;

        std::getline(std::cin, userInput);
        if(userInput == "exit")
        exit(0);
        
        //
        std::queue<std::string> qCommands;

        std::cout << qCommands.size() << std::endl;

        //vecStrings holds all the commands 
        std::vector<std::vector<std::string> > 
            userCommands = splitCommands(userInput, qCommands);

        std::cout << "Overall size: " << userCommands.size() << std::endl;
        for(int i = 0; i < userCommands.size(); i++)
        {
            for(int j = 0; j < userCommands.at(i).size(); j++)
            {
                std::cout << userCommands.at(i).at(j) << std::endl;
            }
            std::cout << "Size: " << userCommands.at(i).size() << std::endl;
        }


        std::vector<char* > test = stringToChar(userCommands.at(0));
        test.push_back(NULL);
        char **command = &test[0];

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
            int status = execvp(command[0], command);
        
            perror("execvp failed");
        }

        return 0;
}



    //char* args[argc];
    //for(int i = 1; i < argc; i++)
    //{
    //    args[i - 1] = argv[i];
    //}
    //args[argc - 1] = NULL;

    //pid_t processID;
    //int status;

    //processID = fork();
    
    //if(processID < 0)
    //{
    //    perror("fork failed");
    //    exit(1);
    //}

    //else if(processID == 0)
    //{
    //    execvp(args[0], args);
//
    //    for(int i = 0; i < argc - 1; i++)
    //    {
    //        std::cout << args[i] << std::endl;
    //    }
//
    //    perror("execvp failed");
    //}
    //else
    //{
    //   if((processID = wait(&status)) < 0)
    //    {
    //        perror("wait");
    //        exit(1);
    //    }
//
 //       std::cout << "hello" << std::endl;
//    }

// Splits a string of sentences to a vector of words
std::vector<std::vector<std::string> > splitCommands
    (std::string str, std::queue<std::string>& q)
{
    std::vector<std::vector<std::string> > vectorWords;
    std::vector<std::string> emptyVector;
    vectorWords.push_back(emptyVector);
    int index = 0;

    std::istringstream iSS(str);
    do
    {
        std::string singleWord;
        iSS >> singleWord;

        // Checks if there is any semicolon on the end of the word
        // and separate it
        if(*singleWord.rbegin() == ';' && singleWord != ";")
        {
            singleWord = singleWord.substr(0, singleWord.size() - 1);
            vectorWords.at(index).push_back(singleWord);
            q.push(";");
            index++;
            vectorWords.push_back(emptyVector);
        }
        else if(singleWord == "||" || singleWord == "&&" || singleWord == ";")
        {
            q.push(singleWord);
            index++;
            vectorWords.push_back(emptyVector);
        }
        else
            vectorWords.at(index).push_back(singleWord);

    } while(iSS);

    for(int i = 0; i < vectorWords.size(); i++)
    {
        vectorWords.at(i).pop_back();
    }
   
    return vectorWords;
}

bool isConnector(std::string s)
{
    if(s == "&&" || s == "||" || s == ";")
        return true;
    
    return false;
}

std::vector<char *> stringToChar(std::vector<std::string>& s)
{
    std::vector<char *> c;

    for(int i = 0; i < s.size(); i++)
    {
        c.push_back(&s[i][0]);
    }
    return c;
}
