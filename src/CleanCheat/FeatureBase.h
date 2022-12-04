#pragma once
#include "Macros.h"
#include "FeatureSettings.h"

template <typename TType, class TSettings = FeatureSettings>
ABSTRACT class FeatureBase
{
private:
    bool _init = false;

public:
    TSettings* Settings;

public:
    virtual ~FeatureBase() = default;

protected:
    virtual void OnExecute(TType* param) = 0;

    virtual bool OnInit(void* initData)
    {
        return true;
    }

public:
    /// <summary>
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const
    {
        return _init;
    }
    
    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition(TType* param) = 0;

    /// <summary>
    /// Called before runner executive
    /// </summary>
    virtual void BeforeExecute() { }

    /// <summary>
    /// Called after runner executive
    /// </summary>
    virtual void AfterExecute() { }

    /// <summary>
    /// Called during runner executive once or more depend on the runner
    /// </summary>
    void Execute(TType* param)
    {
        if (!_init)
            return;

        OnExecute(param);
    }

    /// <summary>
    /// Initialize
    /// </summary>
    bool Init(void* initData = nullptr)
    {
        if (_init)
            return false;
        _init = true;

        Settings = new TSettings();
        
        return Settings->Init() && OnInit(initData);
    }
     
    /// <summary>
    /// Discard
    /// </summary>
    void Discard()
    {
        OnDiscard();
        DELETE_HEAP(Settings);
    }

    /// <summary>
    /// Called by discard method
    /// </summary>
    virtual void OnDiscard() { }
};
