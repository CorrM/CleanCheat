#pragma once
#include "Macros.h"
#include "FeatureSettings.h"
#include "RunnerTaskBase.h"

template <typename TType, class TSettings = FeatureSettings, typename TInitDataType = void>
ABSTRACT class FeatureBase : public RunnerTaskBase<TType, TInitDataType>
{
public:
    TSettings Settings{};

private:
    bool OnInitialize(TInitDataType* initData = nullptr) override
    {
        return Settings.Init() && OnInit();
    }

    void OnDestroy() override
    {
        OnDiscard();
    }

public:
    virtual bool OnInit(TInitDataType* initData = nullptr)
    {
        return true;
    }

    virtual void OnDiscard() { }
};
