#pragma once
#include "CleanCheat/RunnerBase.h"

class BasicRunner final : public RunnerBase<int>
{
protected:
    void OnExecute() override;
    
public:
    std::string Name() override;
    bool Condition() override;
};
