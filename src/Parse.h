#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <queue>
#include <boost/tokenizer.hpp>
#include <utility>
#include <stack>

class Parse
{
    private:
        static std::queue<std::string> parseHelper(std::string s,
                boost::char_separator<char> sep);
        static std::vector<std::string> parseSpaces(std::string s);
        static std::vector<std::string> splitHash
            (const std::vector<std::string>& v);
        static void parseQuotes(std::vector<std::string>& v);

    public:
        static std::queue<std::string> parseCommand(std::string s);
        static std::queue<std::string> parseConnector(std::string s, bool &b);
        static std::vector<std::string> prepareVector(std::string s);
        static bool errorLeadingConnector(std::string s);
};

#endif
