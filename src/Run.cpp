#include "Run.h"

std::vector<char*> Run::toCharPointers(std::vector<std::string>& v)
{
    std::vector<char*> c;
    for (unsigned i = 0; i < v.size(); i++)
        c.push_back(&v[i][0]);

    return c;
}

bool Run::executeSingle(std::vector<std::string>& v)
{
    std::vector<char*> vecChars = toCharPointers(v);
    vecChars.push_back(NULL);
    char** listCommands = &vecChars[0];

    // Start execvp process
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
    else
        if((processID = wait(&status)) < 0)
        {
            perror("wait failed");
            exit(1);
        }
    if(WIFEXITED(status))
        exeVal = WEXITSTATUS(status);

    if(exeVal == 0)
        return true;

    return false;
}

void Run::executeAll(std::queue<std::string>& qCmd, 
        std::queue<std::string>& qCnct)
{
    if(qCmd.empty())
        return;

    std::vector<std::string> splitParams = Parse::prepareVector(qCmd.front()); 

    bool correctlyExecuted = executeSingle(splitParams);
    qCmd.pop();
    
    while (!qCmd.empty())
    {
        splitParams = Parse::prepareVector(qCmd.front());
        if (qCnct.front() == "&&")
        {
            And a(correctlyExecuted);
            if(a.executeNext())
                correctlyExecuted = executeSingle(splitParams);
        }
        else if (qCnct.front() == "||")
        {
            Or o(correctlyExecuted);
            if(o.executeNext())
                correctlyExecuted = executeSingle(splitParams);
        }
        else if (qCnct.front() == ";")
        {
            Semicolon sc(correctlyExecuted);
            if (sc.executeNext())
                correctlyExecuted = executeSingle(splitParams);
        }
        qCnct.pop();
        qCmd.pop();
    }
}

// false for no error
// true for error
bool Run::errorOnLeadingConnector(std::string userInput)
{
    // No errors can be possible if it is not a connector
    if(userInput.size() != 1 && userInput.size() != 2)
        return false;
    
    std::istringstream iSS(userInput);

    std::string front;
    iSS >> front;
    
    std::string::iterator it = front.begin();

    if(userInput.size() == 1 && *it == ';')
    {
        return true;
    }

    if(*it == '&' )
    {
        it++;
        if(*it == '&')
            return true;
        it--;
    }
    else if(*it == '|')
    {
        it++;
        if(*it == '|')
            return true;
        it--;
    }

    return false;
}

void Run::start(std::string userInput)
{
    //check for leading
    if(errorOnLeadingConnector(userInput))
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return;
    }

    std::queue<std::string> qCommands = Parse::parseCommand(userInput);

    bool err = false;
    std::queue<std::string> qConnectors = Parse::parseConnector(userInput, err);
    if(err)
    {
        std::cout << "Error: Invalid Syntax." << std::endl;
        return;
    }
    
    //start test here
    std::queue<std::string> qNigga = qCommands;
    while(!qNigga.empty())
    {
        std::cout << qNigga.front() <<  std::endl;
        qNigga.pop();
    }
    std::cout << "--------------------" << std::endl;

    while(!qConnectors.empty())
    {
        std::cout << qConnectors.front() << std::endl;
        qConnectors.pop();
    }
    std::cout << "------------------------" << std::endl;

    std::vector<std::string> yolo = Parse::prepareVector(qCommands.front());
    for(unsigned i = 0; i < yolo.size(); i++)
        std::cout << yolo.at(i) << std::endl;
     //end test there

    //executeAll(qCommands, qConnectors);
}
