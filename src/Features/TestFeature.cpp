#include <iostream>
#include "TestFeature.h"

void TestFeature::OnExecute(int* param)
{
    std::cout << *param << " From TestFeature" << std::endl;
}

std::wstring TestFeature::Name()
{
    return L"TestFeature";
}

bool TestFeature::Condition(int* param)
{
    _flip = !_flip;
    return _flip;
}
