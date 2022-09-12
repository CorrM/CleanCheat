#pragma once
#include <vector>
#include "RunnerBase.h"
#include "SharedDataBase.h"

class CleanCheat final
{
private:
    static bool _init;
    static std::vector<RunnerBase<void>*> _runners;
    static SharedDataBase* _sharedData;

public:
    template <class TSharedData>
    static TSharedData* GetSharedData();

    static void Init(SharedDataBase* sharedData);

    template <class TRunner>
    static bool RegisterRunner(TRunner* runner);

    static void Tick(void* sharedDataParam);
    static void Clear();
};

template <class TSharedData>
TSharedData* CleanCheat::GetSharedData()
{
    return reinterpret_cast<TSharedData*>(_sharedData);
}

template <class TRunner>
bool CleanCheat::RegisterRunner(TRunner* runner)
{
    if (!_init)
        return false;

    _runners.push_back(reinterpret_cast<RunnerBase<void>*>(runner));
    
    return true;
}
