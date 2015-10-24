#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string>
#include <errno.h>

std::pair<std::vector<std::vector<std::string> >, std::queue<std::string> >
    parseInput(std::string s);
void printCommands(const std::vector<std::vector<std::string> >& v);
void printConnectors(std::queue<std::string> q);
bool executeSingle(std::vector<std::string> v);
std::vector<char*> toCharPointers(std::vector<std::string>& v);

int main()
{
    for(;;)
    {
        // Get user input
        std::string userInput;
        std::getline(std::cin, userInput);
        
        // If user inputs exit, end program
        if(userInput == "exit")
            exit(0);
    
        // parsedPair holds the list of commands on its .first, and holds
        // the list of connectors on its .second
        std::pair<std::vector<std::vector<std::string> >, 
            std::queue<std::string> > parsedPair = parseInput(userInput);

        // correctedExecuted = 1 if executeSingle returned with no errors.
        bool correctlyExecuted = executeSingle(parsedPair.first.at(0));
    }

    return 0;
}

// Returns a pair of vector<vector<string>> and queue<string>
// The vector<vector<string>> is a 3D vector that sorts each execution into 
// a outer vector, with all the separate words in the inner vector.
// The queue<string> stores all the connectors.
std::pair<std::vector<std::vector<std::string> >, std::queue<std::string> >
    parseInput(std::string s)
{
    // Initialize vecCommands with an emptyVector (size = 1)
    std::vector<std::vector<std::string> > vecCommands;
    std::vector<std::string> emptyVector;
    vecCommands.push_back(emptyVector);
    // index will keep track of what index the vecCommands vector will be in
    int index = 0;

    std::queue<std::string> queueConnectors;

    // Read from string until completely parsed
    std::istringstream iSS(s);
    do
    {
        // Store one word into variable singleWord
        std::string singleWord;
        iSS >> singleWord;

        // If ; is at the end of any word, add it into the connectors queue and
        // add the word without the semicolon into vecCommands
        if(*singleWord.rbegin() == ';' && singleWord.size() > 1)
        {
            singleWord = singleWord.substr(0, singleWord.size() - 1);
            vecCommands.at(index).push_back(singleWord);
            queueConnectors.push(";");
            
            // Connector means the current execution is over and a new 
            // action will be taking place.
            index++;
            vecCommands.push_back(emptyVector);
        }
        // If a word consists of only ||, &&, or ; add into connectors queue
        else if(singleWord == "||" || singleWord == "&&" || singleWord == ";")
        {
            queueConnectors.push(singleWord);
            
            // Connector means the current execution is over and a new
            // action will be taking place.
            index++;
            vecCommands.push_back(emptyVector);
        }
        // In the case of a regular word, simply push into vector of commands
        else
            vecCommands.at(index).push_back(singleWord);
        
    } while(iSS.good());
    
    return std::make_pair(vecCommands, queueConnectors);
}

// Returns the contents of a whole 3D vector array. Used for testing.
void printCommands(const std::vector<std::vector<std::string> >& v)
{
    std::cout << "Length of outer vector: " << v.size() << std::endl;
    for(int i = 0; i < v.size(); i++)
    {
        std::cout << "Length of inner vector: " << v.at(i).size() << std::endl;
        for(int j = 0; j < v.at(i).size(); j++)
        {
            std::cout << v.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
}

// Returns the contents of a queue of strings. Used for testing.
void printConnectors(std::queue<std::string> q)
{
    std::cout << "Length of queue: " << q.size() << std::endl;
    while(!q.empty())
    {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
}

// Execute execvp on one command (a single vector of strings)
bool executeSingle(std::vector<std::string> v)
{
    // Prepare char** object to be passed into execvp
    std::vector<char*> vecChars = toCharPointers(v);
    vecChars.push_back(NULL);
    char** listCommands = &vecChars[0];
    
    // Start execvp process
    int status;
    int exeVal;
    pid_t processID = fork();
    if(processID < 0)   // processID of less than 0 means the fork failed
    {
        perror("fork failed");
        exit(1);
    }
    // Child Process
    else if(processID == 0)
    {
        execvp(listCommands[0], listCommands);
        perror("execvp failed");
        exit(errno);
    }
    // Parent Process
    else
    {
        // Wait until child process is finished.
        // If wait(&status) is less than 0, means wait syscall failed.
        if((processID = wait(&status)) < 0)
        {
            perror("wait");
            exit(1);
        }
        
        // set exeVal to how the child process terminated.
        if(WIFEXITED(status))
        {
            exeVal = WEXITSTATUS(status);
        }
        
        // exeVal == 0 means that the child process successfully ran
        // Otherwise, that means there was an error with the process.
        if(exeVal == 0)
            return true;
        return false;
    }
}

// Creates a vector of character pointers based on the passed in
// vector of strings.
std::vector<char*> toCharPointers(std::vector<std::string>& v)
{
    std::vector<char *> c;
    for(int i = 0; i < v.size(); i++)
        c.push_back(&v[i][0]);

    return c;
}
