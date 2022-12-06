#include "BasicDataProvider.h"

void BasicDataProvider::OnExecute(int* param)
{
    SetData(&_secretName);
}

std::string BasicDataProvider::Name()
{
    return "BasicDataProvider";
}

bool BasicDataProvider::Condition(int* param)
{
    return true;
}
