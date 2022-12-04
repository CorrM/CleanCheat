#pragma once
#include "Macros.h"
#include "DataProviderOptionsBase.h"
#include "RunnerTaskBase.h"

template <typename TInputType, typename TOutDataType, class TOptions = DataProviderOptionsBase>
ABSTRACT class DataProviderBase : public RunnerTaskBase<TInputType>
{
private:
    TOutDataType* _data = nullptr;
    
protected:
    TOptions _options{};
    
protected:
    /// <summary>
    /// Set data pointer to point data this class provide
    /// </summary>
    /// <remarks
    /// DON'T use stack allocated data, as that will just store a pointer to your data
    /// </remarks>
    void SetData(TOutDataType* newData)
    {
        _data = newData;
    }

    void OnDestroy() override
    {
        OnDiscard();
    }
    
public:
    virtual void OnDiscard() { }

    /// <summary>
    /// Get options of this data provider, You can change its options to get
    /// different output data
    /// </summary>
    TOptions& GetOptions()
    {
        return _options;
    }
    
    /// <summary>
    /// Get a pointer to data this class provide
    /// </summary>
    TOutDataType* GetData()
    {
        return _data;
    }
};
