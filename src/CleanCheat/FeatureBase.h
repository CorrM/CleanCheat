#pragma once
#include "Macros.h"

template <typename TType>
ABSTRACT class FeatureBase
{
private:
    bool _init = false;

public:
    virtual ~FeatureBase() = default;
    virtual void Discard() { }

protected:
    virtual void OnExecute(TType* param) = 0;

    virtual bool OnInit(void* initData)
    {
        return true;
    }

public:
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
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const
    {
        return _init;
    }

    /// <summary>
    /// Initialize
    /// </summary>
    bool Init(void* initData = nullptr)
    {
        if (_init)
            return false;
        _init = true;

        return OnInit(initData);
    }
};
