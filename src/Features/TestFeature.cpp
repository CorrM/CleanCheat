#include <iostream>
#include "TestFeature.h"

void TestFeature::OnExecute(int* param)
{
    std::cout << *param << " From TestFeature" << std::endl;
}

bool TestFeature::Condition(int* param)
{
    _flip = !_flip;
    return _flip;
}
