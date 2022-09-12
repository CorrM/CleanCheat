#pragma once
#include "CleanCheat/RunnerBase.h"

class LevelActorsRunner final : public RunnerBase
{
protected:
    void OnExecute() override;
    
public:
    bool Condition() override;
};
