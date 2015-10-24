#include "Or.h"

bool Or::executeNext()
{
    if(getPrevious())
        return false;
    
    return true;
}
