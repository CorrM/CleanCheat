#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "../Detours/detours.h"
#include "CleanCheat.h"

#include <thread>

bool CleanCheat::_init = false;
bool CleanCheat::_busy = false;
std::vector<RunnerBase*> CleanCheat::_runners;
std::unordered_map<void**, void*> CleanCheat::_detours;
SharedDataBase* CleanCheat::_sharedData = nullptr;

void CleanCheat::Init(SharedDataBase* sharedData)
{
    if (_init)
        return;
    _init = true;
    
    _sharedData = sharedData;
}

void CleanCheat::Tick(void* sharedDataParam)
{
    if (!_init)
        return;

    _busy = true;
    _sharedData->Tick(sharedDataParam);
    
    for (RunnerBase*& runner : _runners)
    {
        if (runner->Condition())
            runner->Tick();
    }
    
    _busy = false;
}

void CleanCheat::Dispose(const bool delSharedData, const bool delRunners, const bool delFeatures)
{
    _init = false;

    // Wait Tick to finish
    while (_busy)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    // Clear runners
    if (delRunners || delFeatures)
    {
        for (RunnerBase* runner : _runners)
        {
            if (delFeatures)
                runner->DeleteFeatures();
            
            if (delRunners)
                delete runner;
        }
        
        _runners.clear();
    }

    // Un hook all functions
    UnDetourAll();
    
    if (delSharedData)
    {
        delete _sharedData;
        _sharedData = nullptr;
    }
}

void CleanCheat::SwapVmtFunction(void* instance, const int32_t vmtIndex, void* hkFunc, void** outOriginalFunc)
{
    void** index = *static_cast<void***>(instance);
    if (outOriginalFunc)
        *outOriginalFunc = index[vmtIndex];

    DWORD virtualProtect;
    VirtualProtect(&index[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
    index[vmtIndex] = hkFunc;
    VirtualProtect(&index[vmtIndex], 0x8, virtualProtect, &virtualProtect);
}

void CleanCheat::UnSwapVmtFunction(void* instance, const int32_t vmtIndex, void* originalFunc)
{
    void** index = *static_cast<void***>(instance);

    DWORD virtualProtect;
    VirtualProtect(&index[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
    index[vmtIndex] = originalFunc;
    VirtualProtect(&index[vmtIndex], 0x8, virtualProtect, &virtualProtect);
}

void CleanCheat::DetourFunction(void** originalFunctionPointer, void* detourPointer)
{
    _detours.emplace(originalFunctionPointer, detourPointer);
    
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(originalFunctionPointer, detourPointer);
    DetourTransactionCommit();
}

void CleanCheat::UnDetourFunction(void** originalFunctionPointer, void* detourPointer)
{
    _detours.erase(originalFunctionPointer);
    
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(originalFunctionPointer, detourPointer);
    DetourTransactionCommit();
}

void CleanCheat::UnDetourAll()
{
    for (std::pair<void** const, void*>& detour : _detours)
    {
        void** originalFn = detour.first;
        void* detourFn = detour.second;

        UnDetourFunction(originalFn, detourFn);
    }
}