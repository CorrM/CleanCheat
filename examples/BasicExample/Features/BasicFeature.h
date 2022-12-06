#pragma once
#include "CleanCheat/FeatureBase.h"

class BasicSettings final : public FeatureSettings
{
public:
    bool Test = false;
};

class BasicFeature final : public FeatureBase<int, BasicSettings, int>
{
protected:
    void OnExecute(int* param) override;
    
public:
    std::string Name() override;
    bool Condition(int* param) override;
};
