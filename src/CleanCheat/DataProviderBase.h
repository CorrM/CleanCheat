#pragma once
#include "Macros.h"
#include "RunnerTaskBase.h"

template <typename TInputType, typename TOutDataType>
ABSTRACT class DataProviderBase : public RunnerTaskBase<TInputType>
{
private:
    TOutDataType* _data = nullptr;

protected:
    /// <summary>
    /// Set data pointer to point data that class provide
    /// </summary>
    /// <remarks
    /// DON'T use stack allocated data, as that will just store a pointer to your data
    /// </remarks>
    void SetData(TOutDataType* newData)
    {
        _data = newData;
    }
    
public:
    /// <summary>
    /// Get a pointer to data that class provide
    /// </summary>
    TOutDataType* GetData()
    {
        return _data;
    }
};
