#ifndef CONNECTOR_H
#define CONNECTOR_H

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
