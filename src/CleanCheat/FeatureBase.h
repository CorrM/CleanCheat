#pragma once
#include "Macros.h"

template <typename TType>
ABSTRACT class FeatureBase
{
private:
    bool _isInitialized = false;

public:
    virtual ~FeatureBase() = default;

protected:
    /// <summary>
    /// Called only once
    /// </summary>
    virtual void OnInit(TType* param);

public:
    /// <summary>
    /// Condition runner will use to determine will execute this feature or not
    /// </summary>
    virtual bool Condition() = 0;

    /// <summary>
    /// Called before runner executive
    /// </summary>
    virtual void BeforeExecute();

    /// <summary>
    /// Called during runner executive once or more depend on runner
    /// </summary>
    virtual void Execute(TType* param) = 0;

    /// <summary>
    /// Called after runner executive
    /// </summary>
    virtual void AfterExecute();
    
    /// <summary>
    /// Determinate initialization status
    /// </summary>
    /// <returns>`True` if initialized, otherwise `False`</returns>
    bool IsInitialized() const;

    /// <summary>
    /// Called only once
    /// </summary>
    void Init(TType* param = nullptr);
};

template <typename TType>
void FeatureBase<TType>::OnInit(TType* param) { }

template <typename TType>
bool FeatureBase<TType>::IsInitialized() const
{
    return _isInitialized;
}

template <typename TType>
void FeatureBase<TType>::Init(TType* param)
{
    if (_isInitialized)
        return;
    _isInitialized = true;

    OnInit(param);
}

template <typename TType>
void FeatureBase<TType>::BeforeExecute() {}

template <typename TType>
void FeatureBase<TType>::AfterExecute() {}
