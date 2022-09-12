#pragma once
#include <vector>

#include "Macros.h"
#include "FeatureBase.h"

template <class TParam>
ABSTRACT class RunnerBase
{
protected:
    std::vector<FeatureBase<void>*> _features;

public:
    virtual ~RunnerBase() = default;
    
private:
    void ExecuteBeforeCallbacks() const;
    void ExecuteAfterCallback() const;

protected:
    virtual void OnExecute() = 0;

    /// <summary>
    /// Execute registered features
    /// </summary>
    void ExecuteFeatures(TParam* item) const;

public:
    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition() = 0;
    
    /// <summary>
    /// Called evey tick
    /// </summary>
    void Tick();

    /// <summary>
    /// Register feature
    /// </summary>
    template <class TFeature>
    void RegisterFeature(TFeature* feature);
};

template <class TParam>
void RunnerBase<TParam>::ExecuteBeforeCallbacks() const
{
    for (FeatureBase<void>* const& feature : _features)
        feature->BeforeExecute();
}

template <class TParam>
void RunnerBase<TParam>::ExecuteFeatures(TParam* item) const
{
    for (FeatureBase<void>* const& feature : _features)
    {
        if (feature->Condition())
            feature->Execute(item);
    }
}

template <class TParam>
void RunnerBase<TParam>::ExecuteAfterCallback() const
{
    for (FeatureBase<void>* const& feature : _features)
        feature->AfterExecute();
}

template <class TParam>
void RunnerBase<TParam>::Tick()
{
    ExecuteBeforeCallbacks();
    OnExecute();
    ExecuteAfterCallback();
}

template <class TParam>
template <class TFeature>
void RunnerBase<TParam>::RegisterFeature(TFeature* feature)
{
    _features.push_back(reinterpret_cast<FeatureBase<void>*>(feature));
}
