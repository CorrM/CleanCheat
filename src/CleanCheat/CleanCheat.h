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
    template <class TSharedData>
    static TSharedData* GetSharedData();

    static void Init(SharedDataBase* sharedData);

    template <class TRunner>
    static bool RegisterRunner(TRunner* runner);

    static void Tick(void* sharedDataParam);
    static void Dispose(bool delSharedData, bool delRunners, bool delFeatures);
    
    static void SwapVmtFunction(void* instance, int32_t vmtIndex, void* hkFunc, void** outOriginalFunc);
    static void UnSwapVmtFunction(void* instance, int32_t vmtIndex, void* originalFunc);
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

    _runners.push_back(reinterpret_cast<RunnerBase*>(runner));

    return true;
}
