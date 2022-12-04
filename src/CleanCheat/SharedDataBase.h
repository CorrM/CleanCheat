#pragma once
#include "Macros.h"

template <typename TInit>
ABSTRACT class SharedDataBase
{
public:
    virtual ~SharedDataBase() = default;
    
public:
    /// <summary>
    /// Called every tick
    /// </summary>
    virtual void Tick(TInit* data) = 0;
};
