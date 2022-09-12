#pragma once
#include "Macros.h"

template <typename TType>
ABSTRACT class FeatureBase
{
private:
    bool _init = false;

public:
    virtual ~FeatureBase() = default;

protected:
    virtual void OnInit(void* initData);
    virtual void OnExecute(TType* param) = 0;
    
public:
    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition(TType* param) = 0;

    /// <summary>
    /// Called before runner executive
    /// </summary>
    virtual void BeforeExecute();
    
    /// <summary>
    /// Called after runner executive
    /// </summary>
    virtual void AfterExecute();

    /// <summary>
    /// Called during runner executive once or more depend on the runner
    /// </summary>
    void Execute(TType* param);
    
    /// <summary>
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const;

    /// <summary>
    /// Initialize
    /// </summary>
    void Init(void* initData = nullptr);
};

template <typename TType>
void FeatureBase<TType>::OnInit(void* initData) { }

template <typename TType>
bool FeatureBase<TType>::IsInitialized() const
{
    return _init;
}

template <typename TType>
void FeatureBase<TType>::Init(void* initData)
{
    if (_init)
        return;
    _init = true;

    OnInit(initData);
}

template <typename TType>
void FeatureBase<TType>::BeforeExecute() {}

template <typename TType>
void FeatureBase<TType>::Execute(TType* param)
{
    if (!_init)
        return;

    OnExecute(param);
}

template <typename TType>
void FeatureBase<TType>::AfterExecute() {}
