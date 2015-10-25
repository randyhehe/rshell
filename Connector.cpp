#include "Connector.h"

Connector::Connector(bool b)
{
    previous = b;
}

bool Connector::getPrevious()
{
    return previous;
}
