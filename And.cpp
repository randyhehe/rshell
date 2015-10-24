#include "And.h"

bool And::executeNext()
{
    if(getPrevious())
        return false;

    return true;
}
