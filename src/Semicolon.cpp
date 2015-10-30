#include "Semicolon.h"

Semicolon::Semicolon(bool b)
:Connector(b)
{}

bool Semicolon::executeNext()
{
    return true;
}
