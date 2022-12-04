#pragma once
#include "Macros.h"
#include "FeatureSettings.h"
#include "RunnerTaskBase.h"

template <typename TType, class TSettings = FeatureSettings>
ABSTRACT class FeatureBase : public RunnerTaskBase<TType>
{
public:
    TSettings* Settings;

private:
    bool OnInitialize(void* initData = nullptr) override
    {
        Settings = new TSettings();
        return Settings->Init() && OnInit();
    }

    void OnDestroy() override
    {
        OnDiscard();
        DELETE_HEAP(Settings);
    }

public:
    virtual bool OnInit(void* initData = nullptr)
    {
        return true;
    }

    virtual void OnDiscard() { }

public:
    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition(TType* param) = 0;
};
