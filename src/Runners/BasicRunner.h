#pragma once
#include "CleanCheat/RunnerBase.h"

#include "DataProviders/BasicDataProvider.h"

#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"

// Features
class BasicRunnerFeatures final : public RunnerFeaturesCollectionBase
{
public:
    BasicFeature* Basic = new BasicFeature();
    TestFeature* Test = new TestFeature();
};

// DataProviders
class BasicRunnerDataProviders final : public RunnerDataProvidersCollectionBase
{
public:
    BasicDataProvider* Basic = new BasicDataProvider();
};


class BasicRunner final : public RunnerBase<int, BasicRunnerFeatures, BasicRunnerDataProviders>
{
protected:
    void OnExecute() override;
    
public:
    std::string Name() override;
    bool Condition() override;
};
