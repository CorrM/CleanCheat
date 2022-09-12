#include <iostream>
#include "TestFeature.h"

bool flip = false;

bool TestFeature::Condition()
{
    flip = !flip;
    return flip;
}

void TestFeature::Execute(int* param)
{
    std::cout << *param << " From TestFeature" << std::endl;
}
