#pragma once
#include "Macros.h"

ABSTRACT class SharedDataBase
{
protected:
    virtual ~SharedDataBase() = default;
    
public:
    /// <summary>
    /// Called every tick
    /// </summary>
    virtual void Tick(void* data) = 0;
};
