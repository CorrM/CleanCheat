#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <unordered_map>
#include <Windows.h>
#include "Libs/Detours/detours.h"

class HookManager
{
private:
    std::unordered_map<void**, void*> _detours;
    
public:
    void SwapVmt(void* instance, const int32_t vmtIndex, void* hkFunc, void** outOriginalFunc)
    {
        void** index = *static_cast<void***>(instance);
        if (outOriginalFunc)
            *outOriginalFunc = index[vmtIndex];

        DWORD virtualProtect;
        VirtualProtect(&index[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
        index[vmtIndex] = hkFunc;
        VirtualProtect(&index[vmtIndex], 0x8, virtualProtect, &virtualProtect);
    }

    void UnSwapVmt(void* instance, const int32_t vmtIndex, void* originalFunc)
    {
        void** index = *static_cast<void***>(instance);

        DWORD virtualProtect;
        VirtualProtect(&index[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
        index[vmtIndex] = originalFunc;
        VirtualProtect(&index[vmtIndex], 0x8, virtualProtect, &virtualProtect);
    }

    bool Detour(void** originalFunctionPointer, void* detourPointer)
    {
        _detours.emplace(originalFunctionPointer, detourPointer);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(originalFunctionPointer, detourPointer);

        return DetourTransactionCommit() == NO_ERROR;
    }

    bool UnDetour(void** originalFunctionPointer, void* detourPointer)
    {
        _detours.erase(originalFunctionPointer);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(originalFunctionPointer, detourPointer);

        return DetourTransactionCommit() == NO_ERROR;
    }

    void UnDetourAll()
    {
        for (std::pair<void** const, void*>& detour : _detours)
        {
            void** originalFn = detour.first;
            void* detourFn = detour.second;

            UnDetour(originalFn, detourFn);
        }
    }
};
