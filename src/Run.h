#ifndef RUN_H
#define RUN_H

#include <vector>
#include <queue>
#include <iostream>
#include <unistd.h>
#include <utility>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <sstream>
#include <signal.h>

#include "And.h"
#include "Connector.h"
#include "Or.h"
#include "Semicolon.h"
#include "Parse.h"

class Run
{
    private:
        std::vector<char*> toCharPointers(std::vector<std::string> &v);
        bool executeSingle(std::vector<std::string>& v);
        bool executeAll(std::queue<std::string>& qCmd, 
                std::queue<std::string>& qCnct);
        bool errorOnLeadingConnector(std::string s);
        bool runExec(std::vector<std::string>& v);
        bool runStat(std::vector<std::string>& v);
        char* stringToCharPtr(std::string s);

    public:
        bool start(std::string userInput);
};

#endif
