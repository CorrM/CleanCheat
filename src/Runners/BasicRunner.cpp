#include "BasicRunner.h"

int i = 0;

void BasicRunner::OnExecute()
{
    ExecuteTasks(&i);
    ++i;
}

bool BasicRunner::Condition()
{
    return true;
}
