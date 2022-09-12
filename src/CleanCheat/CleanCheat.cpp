#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "../Detours/detours.h"
#include "CleanCheat.h"

bool CleanCheat::_init = false;
std::vector<RunnerBase<void>*> CleanCheat::_runners;
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
    
    _sharedData->Tick(sharedDataParam);
    
    for (RunnerBase<void>*& runner : _runners)
    {
        if (runner->Condition())
            runner->Tick();
    }
}

void CleanCheat::Clear()
{
    _runners.clear();
}

void CleanCheat::SwapVmtFunction(void* instance, void* hkFunc, const int32_t vftIndex, void** outOriginalFunc)
{
    void** index = *static_cast<void***>(instance);
    if (outOriginalFunc)
        *outOriginalFunc = index[vftIndex];

    DWORD virtualProtect;
    VirtualProtect(&index[vftIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
    index[vftIndex] = hkFunc;
    VirtualProtect(&index[vftIndex], 0x8, virtualProtect, &virtualProtect);
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