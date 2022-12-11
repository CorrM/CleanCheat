#pragma once
#include "CleanCheat/RunnerBase.h"
#include "Features/ChamsFeature.h"

class CG::AActor;

// DataProviders
class BasicRunnerDataProviders final : public RunnerDataProvidersCollectionBase
{
public:
};

// Features
class BasicRunnerFeatures final : public RunnerFeaturesCollectionBase
{
public:
    ChamsFeature* Chams = new ChamsFeature();
};

class LevelActorsRunner final : public RunnerBase<CG::AActor, BasicRunnerFeatures, BasicRunnerDataProviders>
{
protected:
    void OnExecute() override;
    
public:
    std::string Name() override;
    bool Condition() override;
};
