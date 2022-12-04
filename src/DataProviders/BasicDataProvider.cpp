#include "BasicDataProvider.h"

void BasicDataProvider::OnExecute(int* param)
{
    SetData(&_secretName);
}

std::wstring BasicDataProvider::Name()
{
    return L"BasicDataProvider";
}
