#include <iostream>

#include "BasicFeature.h"

void BasicFeature::OnExecute(int* param)
{
    std::cout << *param << " From BasicFeature" << std::endl;
}

std::string BasicFeature::Name()
{
    return "BasicFeature";
}

bool BasicFeature::Condition(int* param)
{
    return Settings.Enable;
}
