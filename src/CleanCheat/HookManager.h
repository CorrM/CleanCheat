#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <unordered_map>
#include <Windows.h>
#include "Libs/Detours/detours.h"

class HookManager final
{
private:
    std::unordered_map<void**, void*> _detours;
    std::unordered_map<void*, std::vector<std::pair<int32_t, void*>>> _vmtSwaps;

public:
    void SwapVmt(void* instance, const int32_t vmtIndex, void* hkFunc, void** outOriginalFunc)
    {
        if (_vmtSwaps.find(instance) == _vmtSwaps.end())
            _vmtSwaps.emplace(instance, std::vector<std::pair<int32_t, void*>>());

        void** vmTable = *static_cast<void***>(instance);
        _vmtSwaps[instance].push_back(std::make_pair(vmtIndex, vmTable[vmtIndex]));

        if (outOriginalFunc)
            *outOriginalFunc = vmTable[vmtIndex];

        DWORD virtualProtect;
        VirtualProtect(&vmTable[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
        vmTable[vmtIndex] = hkFunc;
        VirtualProtect(&vmTable[vmtIndex], 0x8, virtualProtect, &virtualProtect);
    }

    void UnSwapVmt(void* instance, const int32_t vmtIndex, void* originalFunc)
    {
        void** vmTable = *static_cast<void***>(instance);

        std::vector<std::pair<int, void*>>::iterator itToRemove = std::remove_if(
            _vmtSwaps[instance].begin(),
            _vmtSwaps[instance].end(),
            [=](const std::pair<int32_t, void*> v)
            {
                return v.first == vmtIndex;
            });
        _vmtSwaps[instance].erase(itToRemove, _vmtSwaps[instance].end());
        if (_vmtSwaps[instance].empty())
            _vmtSwaps.erase(instance);

        DWORD virtualProtect;
        VirtualProtect(&vmTable[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
        vmTable[vmtIndex] = originalFunc;
        VirtualProtect(&vmTable[vmtIndex], 0x8, virtualProtect, &virtualProtect);
    }

    void UnSwapAll()
    {
        for (std::pair<void* const, std::vector<std::pair<int, void*>>>& swap : _vmtSwaps)
        {
            void* instance = swap.first;
            for (std::pair<int, void*>& instanceSwap : swap.second)
            {
                int32_t vmtIndex = instanceSwap.first;
                void* originalFunc = instanceSwap.second;

                UnSwapVmt(instance, vmtIndex, originalFunc);
            }
        }
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

            if (!UnDetour(originalFn, detourFn))
                LOG("ERROR");
        }
    }
};
