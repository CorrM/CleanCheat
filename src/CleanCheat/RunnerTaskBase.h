#pragma once
#include <string>
#include "Macros.h"

template <typename TType, typename TInitDataType = void>
ABSTRACT class RunnerTaskBase
{
private:
    bool _init = false;

public:
    virtual ~RunnerTaskBase() = default;

protected:
    virtual void OnExecute(TType* param) = 0;

    /// <summary>
    /// Called by Init method
    /// </summary>
    virtual bool OnInitialize(TInitDataType* initData)
    {
        return true;
    }

    /// <summary>
    /// Called by Discard method
    /// </summary>
    virtual void OnDestroy() { }
    
public:
    /// <summary>
    /// Get task name
    /// </summary>
    virtual std::string Name() = 0;

    /// <summary>
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const
    {
        return _init;
    }

    /// <summary>
    /// Called before runner executive, Once per frame
    /// </summary>
    virtual void BeforeExecute() { }

    /// <summary>
    /// Called after runner executive, Once per frame
    /// </summary>
    virtual void AfterExecute() { }

    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition(TType* param) = 0;
    
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
    /// Initialize runner task
    /// </summary>
    bool Init(TInitDataType* initData = nullptr)
    {
        if (_init)
            return false;
        _init = true;

        return OnInitialize(initData);
    }
     
    /// <summary>
    /// Discard runner task
    /// </summary>
    void Discard()
    {
        OnDestroy();
    }
};
