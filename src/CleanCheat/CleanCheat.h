#pragma once
#include <vector>
#include <thread>
#include "Macros.h"
#include "RunnerBase.h"
#include "MemoryManager.h"
#include "HookManager.h"

struct CleanCheatOptions
{
public:
    bool UseLogger = false;
};

class CleanCheat final
{
private:
    inline static bool _init = false;
    inline static bool _busy = false;
    inline static CleanCheatOptions _options;
    inline static FILE* _consoleOut = nullptr;
    inline static std::vector<RunnerBase<void>*> _runners;

public:
    inline static SHARED_DATA_TYPE* SharedData = nullptr;
    inline static MemoryManager* Memory = nullptr;
    inline static HookManager* Hook = nullptr;
    
public:
    static void Init(const CleanCheatOptions& options)
    {
        if (_init)
            return;
        _init = true;
        _options = options;

        if (options.UseLogger)
        {
            AllocConsole();
            freopen_s(&_consoleOut, "CONOUT$", "w", stdout);  // NOLINT(cert-err33-c)
        }

        SharedData = new SHARED_DATA_TYPE();
        Memory = new MemoryManager();
        Hook = new HookManager();
    }

    template <class T>
    static bool RegisterRunner(RunnerBase<T>* runner)
    {
        if (!_init)
            return false;

        _runners.push_back(reinterpret_cast<RunnerBase<void>*>(runner));

        return true;
    }

    static void Tick(void* sharedDataTickParam)
    {
        if (!_init)
            return;

        _busy = true;
        SharedData->Tick(sharedDataTickParam);

        for (RunnerBase<void>*& runner : _runners)
        {
            if (runner->Condition())
                runner->Tick();
        }

        _busy = false;
    }

    static void Discard()
    {
        _init = false;

        // Wait Tick to finish
        while (_busy)
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

        // Logger
        if (_options.UseLogger)
        {
            int _ = fclose(_consoleOut);
            _consoleOut = nullptr;
            FreeConsole();
        }

        // Discard runners
        for (RunnerBase<void>* runner : _runners)
            runner->Discard();
        _runners.clear();

        // Un hook all functions
        Hook->UnDetourAll();

        SharedData = nullptr;
        DELETE_HEAP(Memory);
        DELETE_HEAP(Hook);
    }
};
