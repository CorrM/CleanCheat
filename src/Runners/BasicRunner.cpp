#include "BasicRunner.h"

int i = 0;

void BasicRunner::OnExecute()
{
    ExecuteFeatures(&i);
    ++i;
}

bool BasicRunner::Condition()
{
    return true;
}
