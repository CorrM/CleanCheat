#include "BasicRunner.h"

int i = 0;

void BasicRunner::OnExecute()
{
    ExecuteTasks(&i);
    ++i;
}

std::string BasicRunner::Name()
{
    return "BasicRunner";
}

bool BasicRunner::Condition()
{
    return true;
}
