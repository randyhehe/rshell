#ifndef OR_H
#define OR_H

#include "Connector.h"

class Or : public Connector
{
    public:
        Or(bool b);
        bool executeNext();
};

#endif
