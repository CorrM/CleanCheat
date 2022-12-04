#include <iostream>

#include "BasicFeature.h"

void BasicFeature::OnExecute(int* param)
{
    std::cout << *param << " From BasicFeature" << std::endl;
}

std::wstring BasicFeature::Name()
{
    return L"BasicFeature";
}

bool BasicFeature::Condition(int* param)
{
    return Settings->Enable;
}
