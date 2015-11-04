#include "Or.h"

Or::Or(bool b)
:Connector(b)
{}

bool Or::executeNext()
{
    if (getPrevious())
        return false;

    return true;
}
