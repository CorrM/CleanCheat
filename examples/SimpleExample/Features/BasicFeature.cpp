#include <iostream>

#include "BasicFeature.h"

void BasicFeature::OnExecute(int* param)
{
    std::cout << *param << " From BasicFeature" << std::endl;
}

bool BasicFeature::Condition(int* param)
{
    return true;
}
