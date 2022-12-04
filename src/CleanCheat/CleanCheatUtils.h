#pragma once
#include <vector>

class CleanCheatUtils final
{
public:
    template <typename TStruct>
    static std::vector<uintptr_t> CollectPointersAddress(void* structAddress)
    {
        constexpr int sizeOfPointer = sizeof(void*);
        const int structSize = sizeof(TStruct);

        std::vector<uintptr_t> ret;

        if (structSize < sizeOfPointer)
            return ret;

        for (int i = 0; i < structSize; i += sizeOfPointer)
        {
            uintptr_t taskAddress = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(structAddress) + i);
            ret.push_back(taskAddress);
        }

        return ret;
    }
};
