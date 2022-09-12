#include <iostream>

#include "BasicFeature.h"

bool BasicFeature::Condition()
{
    return true;
}

void BasicFeature::Execute(int* param)
{
    std::cout << *param << " From BasicFeature" << std::endl;
}
