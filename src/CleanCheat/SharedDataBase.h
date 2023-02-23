#pragma once
#include "Macros.h"

template <typename TInit>
ABSTRACT class SharedDataBase
{
public:
    virtual ~SharedDataBase() = default;

protected:
    /// <summary>
    /// Called by Discard method
    /// </summary>
    virtual void OnDestroy() { }
    
public:
    /// <summary>
    /// Called every tick
    /// </summary>
    virtual void Tick(TInit* data) = 0;

    /// <summary>
    /// Discard runner task
    /// </summary>
    void Discard()
    {
        OnDestroy();
    }
};
