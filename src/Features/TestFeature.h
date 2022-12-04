#pragma once
#include "CleanCheat/FeatureBase.h"

class TestFeature final : public FeatureBase<int>
{
private:
    bool _flip = false;

protected:
    void OnExecute(int* param) override;
    
public:
    std::string Name() override;
    bool Condition(int* param) override;
};
