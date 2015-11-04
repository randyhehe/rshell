#ifndef CONNECTOR_H
#define CONNECTOR_H

// Pure virtual class Connector has And, Or, and Semicolon class
// inheriting from it. Previous stores the boolean of the previously executed
// execvp, and executeNext will determine whether to execute the next
// execvp by returning true or false.
class Connector
{
    private:
       bool previous;

    public:
       Connector(bool b);
       virtual bool executeNext() = 0;
       bool getPrevious();
};

#endif
