#pragma once
#include <vector>
#include "Macros.h"
#include "FeatureBase.h"

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
    void ExecuteFeatures(void* item) const;

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
    bool RegisterFeature(TFeature* feature);

    /// <summary>
    /// Delete all features (from memory)
    /// </summary>
    void DeleteFeatures();
};

template <class TFeature>
bool RunnerBase::RegisterFeature(TFeature* feature)
{
    if (!feature->IsInitialized())
        return false;

    _features.push_back(reinterpret_cast<FeatureBase<void>*>(feature));

    return true;
}
