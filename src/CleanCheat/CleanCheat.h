#pragma once
#include <unordered_map>
#include <vector>
#include "RunnerBase.h"
#include "SharedDataBase.h"

class CleanCheat final
{
private:
    static bool _init;
    static std::vector<RunnerBase<void>*> _runners;
    static std::unordered_map<void**, void*> _detours;
    static SharedDataBase* _sharedData;

public:
    template <class TSharedData>
    static TSharedData* GetSharedData();

    static void Init(SharedDataBase* sharedData);

    template <class TRunner>
    static bool RegisterRunner(TRunner* runner);

    static void Tick(void* sharedDataParam);
    static void Clear();
    
    static void SwapVmtFunction(void* instance, void* hkFunc, const int32_t vftIndex, void** outOriginalFunc);
    static void DetourFunction(void** originalFunctionPointer, void* detourPointer);
    static void UnDetourFunction(void** originalFunctionPointer, void* detourPointer);
    static void UnDetourAll();
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
