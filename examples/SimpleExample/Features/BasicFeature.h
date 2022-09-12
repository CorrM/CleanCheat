#pragma once
#include "CleanCheat/FeatureBase.h"

class BasicFeature final : public FeatureBase<int>
{
protected:
    void OnExecute(int* param) override;
    
public:
    bool Condition(int* param) override;
};
