#include "Run.h"

// Converts a vector of strings to a vector of character pointers
std::vector<char*> Run::toCharPointers(std::vector<std::string>& v)
{
    std::vector<char*> c;
    for (unsigned i = 0; i < v.size(); i++)
        c.push_back(&v[i][0]);

    return c;
}

// Execute a single command of execvp given a vector of strings.
// Returns true if execution worked, otherwise return false.
bool Run::runExec(std::vector<std::string>& v)
{
    std::vector<char*> vecChars = toCharPointers(v);
    vecChars.push_back(NULL);
    char** listCommands = &vecChars[0];

    int status;
    int exeVal;
    pid_t processID = fork();
    if(processID < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if(processID == 0)
    {
        execvp(listCommands[0], listCommands);
        perror("execvp failed");
        exit(errno);
    }
    else if((processID = wait(&status)) < 0)
    {
        perror("wait failed");
        exit(1);
    }

    // Error check and return boolean value accordingly
    if(WIFEXITED(status))
        exeVal = WEXITSTATUS(status);

    if(exeVal == 0)
        return true;

    return false;
}

char* Run::stringToCharPtr(std::string s)
{
    char* temp = &s.at(0);
    return temp;
}

// Return 0 if the test succeeds and 1 if the test fails
bool Run::runStat(std::vector<std::string>& v)
{
    bool file = false;
    bool dir = false;
    bool all = false;

    if(v.begin() != v.end() && *v.begin() == "[" && *(--v.end()) != "]")
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return 1;
    }

    if(v.begin() != v.end())
        v.erase(v.begin());

    if(v.begin() == v.end())
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return 1;
    }

    if(v.begin() != v.end())
    {
        if(*v.begin() == "-f")
        {   file = true;
            v.erase(v.begin());
        }

        else if(*v.begin() == "-d")
        {
            dir = true;
            v.erase(v.begin());
        }
        else if(*v.begin() == "-e")
        {
            all = true;
            v.erase(v.begin());
        }
        else
            all = true;
    }

    while(v.size() > 1)
    {
        if(v.at(1) == "]")
        {
            v.erase(v.begin() + 1);
            break;
        }
        v.at(0) = v.at(0) + " " + v.at(1);
        v.erase(v.begin() + 1);
    }

    char* input = stringToCharPtr(v.at(0));

    struct stat fileStat;

    if(stat(input, &fileStat) == -1)
        perror("stat");

    // checks for regular file
    if(file)
    {
        switch(fileStat.st_mode & S_IFMT)
        {
            case S_IFREG:
                return 0;
                break;
                
            default:
                return 1;
                break;
        }
    }
    
        // checks for directory
    else if(dir)
    {
        switch(fileStat.st_mode & S_IFMT)
        {
            case S_IFDIR:
                return 0;
                break;
                
            default:
                return 1;
                break;
            }
        }
    
    // checks for either regular file or directory
    else if(all)
    {
        switch(fileStat.st_mode & S_IFMT)
        {
            case S_IFREG:
                return 0;
                break;

            case S_IFDIR:
                return 0;
                break;
                
            default:
                return 1;
                break;
        }
    }
    return 1;
}

bool Run::executeSingle(std::vector<std::string>& v)
{
    // if first string is "test" then do stat instead of execvp
    if(v.at(0) == "test" || v.at(0) == "[")
        return !runStat(v);

    // Start execvp process
    else
        return runExec(v);
}

bool Run::parsePrecedence(std::queue<std::string>& qCmd,
        std::queue<std::string>& qCnct, bool& correctlyExecuted, bool& hasHash)
{
    std::vector<std::string> splitParams = Parse::prepareVector(qCmd.front(), hasHash);

    int numNested = 0;
    char lastChar = splitParams.at(splitParams.size() - 1).at
        (splitParams.at(splitParams.size() - 1).size() - 1);

    do
    {
        if(qCnct.empty() || qCmd.empty())
            return true;

        qCnct.pop();
        qCmd.pop();

        splitParams = Parse::prepareVector(qCmd.front(), hasHash);

        char firstChar = splitParams.at(0).at(0);
        lastChar = splitParams.at(splitParams.size() - 1).at
            (splitParams.at(splitParams.size() - 1).size() - 1);
             
        while(firstChar == '(')
        {
            numNested++;

            splitParams.at(0) = splitParams.at(0).substr(1, splitParams.at(0).size() - 1);
            firstChar = splitParams.at(0).at(0);
        }
        
        bool foundEnd = false; 
        while(lastChar == ')' && numNested > -1)
        {
            numNested--;

            splitParams.at(splitParams.size() - 1) =
                splitParams.at(splitParams.size() - 1).substr
                (0, splitParams.at(splitParams.size() - 1).size() - 1);

            lastChar = splitParams.at(splitParams.size() - 1).at
                (splitParams.at(splitParams.size() - 1).size() - 1);

            foundEnd = true;
        }

        if(numNested == -1 && foundEnd)
            break;

    } while(true);

    return false;
}

// Function attempts to execute all commands.
// If "exit" is found, return false.
// Return true if no exit is found.
// Turn B to true if a syntax error occured.
bool Run::executeAll(std::queue<std::string>& qCmd, 
        std::queue<std::string>& qCnct, bool& b)
{
    if (qCmd.empty())
        return true;

    // hasHash = true if there is a comment in the current preparedVector 
    bool hasHash = false;
    bool correctlyExecuted = false;

    // While there are still commands, keep executing until a comment or exit
    // disrupts the loop.
    while (!qCmd.empty())
    {
        std::vector<std::string> splitParams = Parse::prepareVector(qCmd.front(), hasHash);

        if (qCnct.front() == "&&")
        {
            And a(correctlyExecuted);

            char firstChar = splitParams.at(0).at(0);
            char lastChar = splitParams.at(splitParams.size() - 1).at
                (splitParams.at(splitParams.size() - 1).size() - 1);

            bool frontP = false;
            while(firstChar == '(')
            {
                frontP = true;
                splitParams.at(0) = splitParams.at(0).substr(1, splitParams.at(0).size() - 1);
                firstChar = splitParams.at(0).at(0);
            } 
            
            while(lastChar == ')')
            {
                splitParams.at(splitParams.size() - 1) =
                    splitParams.at(splitParams.size() - 1).substr
                    (0, splitParams.at(splitParams.size() - 1).size() - 1);
                lastChar = splitParams.at(splitParams.size() - 1).at
                    (splitParams.at(splitParams.size() - 1).size() - 1);
            }

            if(!a.executeNext() && frontP)
            {
                bool temp = parsePrecedence(qCmd, qCnct, correctlyExecuted, hasHash);
                if(temp)
                {
                    b = true;
                    return true;
                }
            }

            else if (a.executeNext())
            {
                if (splitParams.at(0) == "exit")
                    return false;

                correctlyExecuted = executeSingle(splitParams);
                if (hasHash) 
                    return true;
            }
        }
        else if (qCnct.front() == "||")
        {
            Or o(correctlyExecuted);
            
            char firstChar = splitParams.at(0).at(0);
            char lastChar = splitParams.at(splitParams.size() - 1).at
                (splitParams.at(splitParams.size() - 1).size() - 1);
            
            bool frontP = false;
            while(firstChar == '(')
            {
                frontP = true;
                splitParams.at(0) = splitParams.at(0).substr(1, splitParams.at(0).size() - 1);
                firstChar = splitParams.at(0).at(0);
            }
            
            while(lastChar == ')')
            {
                splitParams.at(splitParams.size() - 1) =
                    splitParams.at(splitParams.size() - 1).substr
                    (0, splitParams.at(splitParams.size() - 1).size() - 1);
                lastChar = splitParams.at(splitParams.size() - 1).at
                    (splitParams.at(splitParams.size() - 1).size() - 1);
            }

            if(!o.executeNext() && frontP)
            {
                bool temp = parsePrecedence(qCmd, qCnct, correctlyExecuted, hasHash);
                if(temp)
                {
                    b = true;
                    return true;
                }
            }

            else if (o.executeNext())
            {
                if (splitParams.at(0) == "exit")
                    return false;

                correctlyExecuted = executeSingle(splitParams);
                if (hasHash) 
                    return true;
            }
        }
        else if (qCnct.front() == ";")
        {
            Semicolon sc(correctlyExecuted);

            char firstChar = splitParams.at(0).at(0);
            char lastChar = splitParams.at(splitParams.size() - 1).at
                (splitParams.at(splitParams.size() - 1).size() - 1);

            bool frontP = false;
            while(firstChar == '(')
            {
                frontP = true;
                splitParams.at(0) = splitParams.at(0).substr(1, splitParams.at(0).size() - 1);
                firstChar = splitParams.at(0).at(0);
            }
            
            while(lastChar == ')')
            {
                splitParams.at(splitParams.size() - 1) =
                    splitParams.at(splitParams.size() - 1).substr
                    (0, splitParams.at(splitParams.size() - 1).size() - 1);
                lastChar = splitParams.at(splitParams.size() - 1).at
                    (splitParams.at(splitParams.size() - 1).size() - 1);
            }

            if(!sc.executeNext() && frontP)
            {
                bool temp = parsePrecedence(qCmd, qCnct, correctlyExecuted, hasHash);
                if(temp)
                {
                    b = true;
                    return true;
                }
            }

            else if (sc.executeNext())
            {
                if (splitParams.at(0) == "exit")
                    return false;

                correctlyExecuted = executeSingle(splitParams);
                if (hasHash) 
                    return true;
            }
        }
        qCnct.pop();
        qCmd.pop();
    }
    return true;
}

// Checks for leading connectors. False for no error and  true for error.
bool Run::errorOnLeadingConnector(std::string userInput)
{
    // No errors can be possible if it is not a connector
    if (userInput.size() != 1 && userInput.size() != 2)
        return false;
    
    // Read first string into front
    std::istringstream iSS(userInput);
    std::string front;
    iSS >> front;
    
    std::string::iterator it = front.begin();

    if (it == front.end())
        return false;

    if (userInput.size() == 1 && *it == ';')
    {
        return true;
    }

    if (*it == '&' )
    {
        it++;
        if (*it == '&')
            return true;
        it--;
    }
    else if (*it == '|')
    {
        it++;
        if (*it == '|')
            return true;
        it--;
    }

    return false;
}

// Starts the program. Returns true if no exit is found.
// Returns false is exit is found in executeAll function.
bool Run::start(std::string userInput)
{
    //check for leading

    if (errorOnLeadingConnector(userInput))
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return true;
    }

    std::queue<std::string> qCommands = Parse::parseCommand(userInput);
    
    // check for errors when parsing connector
    bool err = false;
    std::queue<std::string> qConnectors = Parse::parseConnector(userInput, err);
    if (err)
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return true;
    }

    // check for errors for leading connector
    if (Parse::errorLeadingConnector(userInput))
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return true;
    }
    
    bool syntaxErr = false;
    bool isExit = executeAll(qCommands, qConnectors, syntaxErr);
    if(syntaxErr)
        std::cout << "Error: Invalid Syntax." << std::endl;

    if (!isExit)
        return false;

    return true;
}
