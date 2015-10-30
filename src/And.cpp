#include "And.h"

And::And(bool b)
:Connector(b)
{}

bool And::executeNext()
{
    if(getPrevious())
        return true;

    return false;
}
