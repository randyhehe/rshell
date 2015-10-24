#ifndef AND_H
#define AND_H

#include "Connector.h"
class And : public Connector
{
    public:
        bool executeNext();
};

#endif
