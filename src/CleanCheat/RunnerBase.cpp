#ifdef USE_PCH
#include "pch.h"
#endif

#include "RunnerBase.h"

void RunnerBase::ExecuteBeforeCallbacks() const
{
    for (FeatureBase<void>* const& feature : _features)
        feature->BeforeExecute();
}

void RunnerBase::ExecuteAfterCallback() const
{
    for (FeatureBase<void>* const& feature : _features)
        feature->AfterExecute();
}

void RunnerBase::ExecuteFeatures(void* item) const
{
    for (FeatureBase<void>* const& feature : _features)
    {
        if (feature->Condition(item))
            feature->Execute(item);
    }
}

void RunnerBase::Tick()
{
    ExecuteBeforeCallbacks();
    OnExecute();
    ExecuteAfterCallback();
}

void RunnerBase::Clear()
{
    _features.clear();
}
