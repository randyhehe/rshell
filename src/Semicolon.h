#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "Connector.h"

class Semicolon : public Connector
{
    public:
        Semicolon(bool b);
        bool executeNext();
};

#endif
