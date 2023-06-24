#pragma once
#include <vector>

#include "Macros.h"
#include "CleanCheatUtils.h"
#include "RunnerFeaturesCollectionBase.h"
#include "RunnerDataProvidersCollectionBase.h"
#include "DataProviderBase.h"
#include "FeatureBase.h"

template <typename TTaskInputType, class TFeatures = RunnerFeaturesCollectionBase, class TDataProviders = RunnerDataProvidersCollectionBase, typename TInitDataType = void>
ABSTRACT class RunnerBase
{
private:
    bool _init = false;

protected:
    std::vector<DataProviderBase<TTaskInputType, void>*> _dataProvidersList;
    std::vector<FeatureBase<TTaskInputType>*> _featuresList;

public:
    TFeatures* Features = new TFeatures();
    TDataProviders* DataProviders = new TDataProviders();

public:
    virtual ~RunnerBase() = default;

private:
    /// <summary>
    /// Call tasks before execute callbacks
    /// </summary>
    void ExecuteBeforeTasksCallbacks() const
    {
        for (DataProviderBase<TTaskInputType, void>* const& dataProvider : _dataProvidersList)
        {
            try
            {
                dataProvider->BeforeExecute();
            }
            catch (...)
            {
                LOG("Runner ERROR (BeforeExecute): DataProvider(%s) throws unhandled exception", dataProvider->Name().c_str());
            }
        }

        for (FeatureBase<TTaskInputType>* const& feature : _featuresList)
        {
            try
            {
                feature->BeforeExecute();
            }
            catch (...)
            {
                LOG("Runner ERROR (BeforeExecute): Feature(%s) throws unhandled exception", feature->Name().c_str());
            }
        }
    }

    /// <summary>
    /// Call tasks after execute callbacks
    /// </summary>
    void ExecuteAfterTasksCallbacks() const
    {
        for (DataProviderBase<void, void>* const& dataProvider : _dataProvidersList)
        {
            try
            {
                dataProvider->AfterExecute();
            }
            catch (...)
            {
                LOG("Runner ERROR (AfterExecute): DataProvider(%s) throws unhandled exception", dataProvider->Name().c_str());
            }
        }

        for (FeatureBase<void>* const& feature : _featuresList)
        {
            try
            {
                feature->AfterExecute();
            }
            catch (...)
            {
                LOG("Runner ERROR (AfterExecute): Feature(%s) throws unhandled exception", feature->Name().c_str());
            }
        }
    }

    /// <summary>
    /// Register data providers
    /// </summary>
    bool RegisterDataProviders()
    {
        _dataProvidersList.clear();
        std::vector<uintptr_t> dataProviders = CleanCheatUtils::CollectPointersAddress<TDataProviders>(DataProviders);
        for (uintptr_t& dataProviderAddress : dataProviders)
        {
            auto* dataProvider = reinterpret_cast<DataProviderBase<TTaskInputType, void>*>(dataProviderAddress);
            if (!dataProvider)
                return false;

            if (!dataProvider->IsInitialized())
                return false;

            _dataProvidersList.push_back(dataProvider);
        }

        return true;
    }

    /// <summary>
    /// Register features
    /// </summary>
    bool RegisterFeatures()
    {
        _featuresList.clear();
        std::vector<uintptr_t> features = CleanCheatUtils::CollectPointersAddress<TFeatures>(Features);
        for (uintptr_t& featureAddress : features)
        {
            auto* feature = reinterpret_cast<FeatureBase<TTaskInputType>*>(featureAddress);
            if (!feature)
                return false;

            if (!feature->IsInitialized())
                return false;

            _featuresList.push_back(feature);
        }

        return true;
    }

protected:
    virtual bool OnInit(TInitDataType* initData = nullptr)
    {
        return true;
    }

    virtual void OnDiscard() { }

    virtual void OnExecute() = 0;

    /// <summary>
    /// Execute registered tasks
    /// </summary>
    void ExecuteTasks(TTaskInputType* item) const
    {
        for (DataProviderBase<TTaskInputType, void>* const& dataProviders : _dataProvidersList)
        {
            if (dataProviders->Condition(item))
            {
                try
                {
                    dataProviders->Execute(item);
                }
                catch (...)
                {
                    LOG("Runner ERROR (ExecuteTasks): DataProvider(%s) throws unhandled exception", dataProviders->Name().c_str());
                }
            }
        }

        for (FeatureBase<TTaskInputType>* const& feature : _featuresList)
        {
            if (feature->Settings.Enable && feature->Condition(item))
            {
                try
                {
                    feature->Execute(item);
                }
                catch (...)
                {
                    LOG("Runner ERROR (ExecuteTasks): Feature(%s) throws unhandled exception", feature->Name().c_str());
                }
            }
        }
    }

public:
    /// <summary>
    /// Get runner name
    /// </summary>
    virtual std::string Name() = 0;

    /// <summary>
    /// Condition CleanCheat will use to determine will execute this runner or not
    /// </summary>
    virtual bool Condition() = 0;

    /// <summary>
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const
    {
        return _init;
    }

    /// <summary>
    /// Initialize runner
    /// </summary>
    bool Init(TInitDataType* initData = nullptr)
    {
        if (_init)
            return false;

        return _init = (RegisterDataProviders() && RegisterFeatures() && OnInit(initData));
    }

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
    /// Discard
    /// </summary>
    void Discard()
    {
        OnDiscard();

        // Features
        for (FeatureBase<void>* feature : _featuresList)
        {
            feature->Discard();
            DELETE_HEAP(feature);
        }
        _featuresList.clear();
        DELETE_HEAP(Features);

        // DataProvider        
        for (DataProviderBase<void, void>* provider : _dataProvidersList)
        {
            provider->Discard();
            DELETE_HEAP(provider);
        }
        _dataProvidersList.clear();
        DELETE_HEAP(DataProviders);
    }
};
