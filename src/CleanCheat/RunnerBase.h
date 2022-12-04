#pragma once
#include <vector>

#include "DataProviderBase.h"
#include "Macros.h"
#include "FeatureBase.h"

template <typename TType>
ABSTRACT class RunnerBase
{
protected:
    std::vector<DataProviderBase<TType, void>*> _dataProviders;
    std::vector<FeatureBase<TType>*> _features;

public:
    virtual ~RunnerBase() = default;

private:
    void ExecuteBeforeTasksCallbacks() const
    {
        for (DataProviderBase<void, void>* const& dataProvider : _dataProviders)
        {
            try
            {
                dataProvider->BeforeExecute();
            }
            catch (...)
            {
                LOG("ERROR (BeforeExecute): DataProvider(%ls) throws unhandled exception", dataProvider->Name().c_str());
            }
        }

        for (FeatureBase<void>* const& feature : _features)
        {
            try
            {
                feature->BeforeExecute();
            }
            catch (...)
            {
                LOG("ERROR (BeforeExecute): Feature(%ls) throws unhandled exception", feature->Name().c_str());
            }
        }
    }

    void ExecuteAfterTasksCallbacks() const
    {
        for (DataProviderBase<void, void>* const& dataProvider : _dataProviders)
        {
            try
            {
                dataProvider->AfterExecute();
            }
            catch (...)
            {
                LOG("ERROR (AfterExecute): DataProvider(%ls) throws unhandled exception", dataProvider->Name().c_str());
            }
        }

        for (FeatureBase<void>* const& feature : _features)
        {
            try
            {
                feature->AfterExecute();
            }
            catch (...)
            {
                LOG("ERROR (AfterExecute): Feature(%ls) throws unhandled exception", feature->Name().c_str());
            }
        }
    }

protected:
    virtual void OnExecute() = 0;

    /// <summary>
    /// Execute registered features
    /// </summary>
    void ExecuteTasks(TType* item) const
    {
        for (DataProviderBase<TType, void>* const& dataProviders : _dataProviders)
        {
            try
            {
                dataProviders->Execute(item);
            }
            catch (...)
            {
                LOG("ERROR (ExecuteTasks): DataProvider(%ls) throws unhandled exception", dataProviders->Name().c_str());
            }
        }

        for (FeatureBase<TType>* const& feature : _features)
        {
            if (feature->Settings->Enable && feature->Condition(item))
            {
                try
                {
                    feature->Execute(item);
                }
                catch (...)
                {
                    LOG("ERROR (ExecuteTasks): Feature(%ls) throws unhandled exception", feature->Name().c_str());
                }
            }
        }
    }

public:
    /// <summary>
    /// Condition CleanCheat will use to determine will execute this runner or not
    /// </summary>
    virtual bool Condition() = 0;

    /// <summary>
    /// Called evey tick
    /// </summary>
    void Tick()
    {
        ExecuteBeforeTasksCallbacks();
        OnExecute();
        ExecuteAfterTasksCallbacks();
    }

    /// <summary>
    /// Register feature
    /// </summary>
    template <class TSettings = FeatureSettings>
    bool RegisterFeature(FeatureBase<TType, TSettings>* feature)
    {
        if (!feature->IsInitialized())
            return false;

        _features.push_back(reinterpret_cast<FeatureBase<TType>*>(feature));

        return true;
    }

    /// <summary>
    /// Register feature
    /// </summary>
    template <class TOut = void>
    bool RegisterDataProvider(DataProviderBase<TType, TOut>* dataProvider)
    {
        if (!dataProvider->IsInitialized())
            return false;

        _dataProviders.push_back(reinterpret_cast<DataProviderBase<TType, void>*>(dataProvider));

        return true;
    }

    /// <summary>
    /// Discard
    /// </summary>
    void Discard()
    {
        for (FeatureBase<void>* feature : _features)
            feature->Discard();

        for (DataProviderBase<void, void>* provider : _dataProviders)
            provider->Discard();

        _features.clear();
        _dataProviders.clear();
    }
};
