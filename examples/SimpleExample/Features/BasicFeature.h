#pragma once
#include "CleanCheat/FeatureBase.h"

class BasicFeature final : public FeatureBase<int>
{
protected:
    bool Condition() override;
    void Execute(int* param) override;
};
