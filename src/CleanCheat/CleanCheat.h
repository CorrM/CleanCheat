#pragma once
#include <unordered_map>
#include <vector>
#include "RunnerBase.h"
#include "SharedDataBase.h"

class CleanCheat final
{
private:
    static bool _init;
    static bool _busy;
    static std::vector<RunnerBase*> _runners;
    static std::unordered_map<void**, void*> _detours;
    static SharedDataBase* _sharedData;

public:
#ifndef SHARED_DATA_TYPE
    template <class TSharedData>
    static TSharedData* GetSharedData();
#else
    static SHARED_DATA_TYPE* GetSharedData();
#endif
    
    static void Init(SharedDataBase* sharedData);

    template <class TRunner>
    static bool RegisterRunner(TRunner* runner);

    static void Tick(void* sharedDataParam);
    static void Clear();
    
    static void SwapVmtFunction(void* instance, int32_t vmtIndex, void* hkFunc, void** outOriginalFunc);
    static void UnSwapVmtFunction(void* instance, int32_t vmtIndex, void* originalFunc);
    static bool DetourFunction(void** originalFunctionPointer, void* detourPointer);
    static bool UnDetourFunction(void** originalFunctionPointer, void* detourPointer);
    static void UnDetourAll();
};

#ifndef SHARED_DATA_TYPE
template <class TSharedData>
TSharedData* CleanCheat::GetSharedData()
{
    return reinterpret_cast<TSharedData*>(_sharedData);
}
#endif

template <class TRunner>
bool CleanCheat::RegisterRunner(TRunner* runner)
{
    if (!_init)
        return false;

    _runners.push_back(reinterpret_cast<RunnerBase*>(runner));

    return true;
}
