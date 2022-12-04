#pragma once
#include "CleanCheat/RunnerBase.h"

class CG::AActor;

class LevelActorsRunner final : public RunnerBase<CG::AActor>
{
protected:
    void OnExecute() override;
    
public:
    std::string Name() override;
    bool Condition() override;
};
