#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <vector>
#include <string>

class MemoryManager
{
private:
    static std::vector<int32_t> PatternToByte(const std::string& pattern)
    {
        std::vector<int32_t> bytes;
        const char* start = pattern.data();
        const char* end = start + pattern.size();
        
        for (const char* current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else
            {
                bytes.push_back(strtoul(current, const_cast<char**>(&current), 16));
            }
        }

        return bytes;
    }

public:
    std::vector<void*> PatternScan(void* moduleBaseAddress, const std::string& signature, const uint32_t maxResultCount = 1)
    {
        std::vector<void*> ret;
        const auto dosHeader = static_cast<PIMAGE_DOS_HEADER>(moduleBaseAddress);
        const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(moduleBaseAddress) + dosHeader->e_lfanew);

        const DWORD sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        std::vector<int32_t> patternBytes = PatternToByte(signature);
        const auto scanBytes = static_cast<uint8_t*>(moduleBaseAddress);

        const int32_t* sigPtr = patternBytes.data();
        const size_t sigSize = patternBytes.size();

        uint32_t foundResults = 0;
        for (uint32_t i = 0; i < sizeOfImage - sigSize; ++i)
        {
            if (maxResultCount != 0 && foundResults == maxResultCount)
                break;
            
            bool found = true;
            for (uint32_t j = 0; j < sigSize; ++j)
            {
                if (scanBytes[i + j] != sigPtr[j] && sigPtr[j] != -1)
                {
                    found = false;
                    break;
                }
            }

            if (found)
            {
                foundResults++;
                ret.push_back(&scanBytes[i]);
            }
        }

        return ret;
    }

    std::vector<void*> PatternScan(const std::string& signature, const uint32_t maxResultCount = 1)
    {
        static bool isSet = false;
        static MODULEINFO info = {0};

        if (!isSet)
        {
            isSet = true;
            GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &info, sizeof(info));
        }

        return PatternScan(info.lpBaseOfDll, signature, maxResultCount);
    }
};
