#include "Parse.h"

typedef boost::tokenizer<boost::char_separator<char> > t_tokenizer;
typedef boost::char_separator<char> cs;

std::queue<std::string> Parse::parseHelper(std::string s, 
        boost::char_separator<char> sep)
{
    std::queue<std::string> que;
    
    t_tokenizer tok(s, sep);
    for (t_tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
        que.push(*beg);

    return que;
}

std::queue<std::string> Parse::parseConnector(std::string s, bool& b)
{
    cs c("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789#\" ");

    std::queue<std::string> que;

    t_tokenizer tok(s, c);

    for (t_tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
    {
        std::string f = *beg;

        // if token is a valid token, push into queue
        if (*beg == ";" || *beg == "&&" || *beg == "||")
        {
            que.push(*beg);
        }
        
        //error if not &&, || or ;
        else if ((f == "|" || f == "&" //|| f.size() > 2
    || (f.size() > 1 && f.at(0) == ';' && f.at(1) == ';')
    || (f.size() > 1 && f.at(0) == '&' && f.at(1) != '&')
    || (f.size() > 1 && f.at(0) == '|' && f.at(1) != '|')
    || (f.size() > 2 && f.at(0) == '&' && f.at(2) == '&')
    || (f.size() > 2 && f.at(0) == '|' && f.at(2) == '|')))
        {
            b = true;
            return que;
        }
    }
    return que;
}

// Return true if leading connector is present
bool Parse::errorLeadingConnector(std::string s)
{
    // Tokenizer for spaces
    cs c(" ");
    t_tokenizer tok(s, c);
    t_tokenizer::iterator beg = tok.begin();
    
    // Do not try to get values from tokenizer if it is empty.
    if (tok.begin() == tok.end())
        return false;

    // If the first thing entered was a connector, return true.
    else if (*beg == "&&" || *beg == "||" || *beg == ";")
        return true;
    
    else if((*beg).size() > 2)
    {
        if((*beg).substr(0, 2) == "&&" || (*beg).substr(0, 2) == "||")
            return true;
    }
    if((*beg).size() > 1)
    {
        if((*beg).substr(0, 1) == ";")
            return true;
    }

    return false;
}
    

std::queue<std::string> Parse::parseCommand(std::string s)
{
    cs sep(";&|");
    std::queue<std::string> que = parseHelper(s, sep);
    
    return que;
}

std::vector<std::string> Parse::parseSpaces(std::string s)
{
    cs sep(" ");

    std::vector<std::string> vec;

    t_tokenizer tok(s, sep);
    for (t_tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
        vec.push_back(*beg);

    return vec;
}

void Parse::parseQuotes(std::vector<std::string>& v)
{
    // iterate through whole vector
    std::stack<std::pair<unsigned, unsigned> > stackPairs;

    for (unsigned i = 0; i < v.size(); i++)
    {
        //if current string at v.at(i) finds a quotation mark
        while (v.at(i).find("\"") != std::string::npos)
        {
            unsigned stringCounter = v.at(i).find("\"");

            // if stack pairs is empty, this means it is the left quote
            if (stackPairs.empty())
            {
                //index of vector into first and index of string at second
                stackPairs.push(std::make_pair(i, stringCounter));
                v.at(i).erase(v.at(i).begin() + stringCounter);

                // if there is text before the left quote, we need to separate
                if(stackPairs.top().second > 0)
                {
                    v.insert(v.begin() + i + 1, v.at(i).substr
                            (stringCounter, v.at(i).size() - stringCounter));
                    v.at(i) = v.at(i).substr(0, stringCounter);
                    stackPairs.top().first++;
                }
            }

            // if stack pairs is not empty, means it is the right quote
            else
            {
                // If right and left quotations are on the same vector index
                if (i == stackPairs.top().first)
                {

                    v.at(i).erase(v.at(i).begin() + stringCounter);
                    stackPairs.pop();
                    
                    // If there are still leading terms, separate into bot vec
                    if (stringCounter != v.at(i).size())
                    {
                        v.insert(v.begin() + i + 1, v.at(i).substr
                            (stringCounter, v.at(i).size() - stringCounter));
                        v.at(i) = v.at(i).substr(0, stringCounter);
                    }
                }
                // When right and left quotations are not on the same vec index
                else if (i != stackPairs.top().first)
                {
                    v.at(i).erase(v.at(i).begin() + stringCounter);

                    // if there are ending terms, separate into bot vec
                    if (stringCounter != v.at(i).size())
                    {
                        v.insert(v.begin() + i + 1, v.at(i).substr
                            (stringCounter, v.at(i).size() - stringCounter));
                        v.at(i) = v.at(i).substr(0, stringCounter);
                    }

                    unsigned currIndex = i;
                    while (currIndex != stackPairs.top().first)
                    {
                        v.at(currIndex - 1) = v.at(currIndex - 1) + " "
                            + v.at(currIndex);
                        v.erase(v.begin() + currIndex);
                        currIndex--;
                    }
                    i = currIndex;
                    stackPairs.pop();
                }

            }
        }
    } 
}


std::vector<std::string> Parse::splitHash(const std::vector<std::string>& v,
        bool& b)
{
    std::vector<std::string> vec;

    for (unsigned i = 0; i < v.size(); i++)
    {
        vec.push_back(v.at(i));

        // If the word is either by itself or has its first letter as #,
        // return the vec before adding in the rest of the elements in v.
        if (*(v.at(i).begin()) == '#')
        {
            vec.pop_back();
            b = true;    // set boolean to true if comment is present
            return vec;
        }
    }
    return vec;
}

std::vector<std::string> Parse::prepareVector(std::string s, bool& b)
{
    std::vector<std::string> retVec = parseSpaces(s);
    retVec = splitHash(retVec, b);
    parseQuotes(retVec);

    return retVec;
}
