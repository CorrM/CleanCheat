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
#ifdef UNICODE
    std::wstring ConsoleTitle = TEXT("CleanCheat");
#else
    std::string ConsoleTitle = TEXT("CleanCheat");
#endif // !UNICODE
};

class CleanCheat final
{
private:
    inline static bool _init = false;
    inline static bool _starts = false;
    inline static bool _busy = false;
    inline static CleanCheatOptions _options;
    inline static std::vector<RunnerBase<void>*> _runners;

public:
    inline static RunnersCollection* Runners = new RunnersCollection();
    inline static SHARED_DATA_TYPE* SharedData = new SHARED_DATA_TYPE();
    inline static MemoryManager* Memory = new MemoryManager();
    inline static HookManager* Hook = new HookManager();

public:
    static bool IsInitialized()
    {
        return _init;
    }
    
    static bool Init(const CleanCheatOptions& options)
    {
        if (_init)
            return false;

        _options = options;
        if (_options.UseLogger)
        {
            if (!AttachConsole(GetCurrentProcessId()))
                AllocConsole();

            freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout); // NOLINT(cert-err33-c)
            SetConsoleTitle(_options.ConsoleTitle.c_str());
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
        }

        return _init = true;
    }

    static bool Start()
    {
        if (_starts)
            return false;
        _starts = true;
        
        // Runner
        _runners.clear();
        std::vector<uintptr_t> runners = CleanCheatUtils::CollectPointersAddress<RunnersCollection>(Runners);
        for (uintptr_t& runnerAddress : runners)
        {
            auto* runner = reinterpret_cast<RunnerBase<void>*>(runnerAddress);
            if (!runner)
                return false;

            if (!runner->IsInitialized())
                return false;

            _runners.push_back(runner);
        }

        return _starts = true;
    }

    template <typename TSharedTickParamType>
    static void Tick(TSharedTickParamType* sharedDataTickParam)
    {
        if (!_init)
            return;

        if (!_starts)
        {
            LOG("You need to start CleanCheat first, Are you forget to call 'Start' function");
            return;
        }
        
        _busy = true;

        try
        {
            SharedData->Tick(sharedDataTickParam);
        }
        catch (...)
        {
            LOG("CleanCheat ERROR (Tick): SharedData throws unhandled exception");
        }

        for (RunnerBase<void>*& runner : _runners)
        {
            if (runner->Condition())
            {
                try
                {
                    runner->Tick();
                }
                catch (...)
                {
                    LOG("CleanCheat ERROR (Tick): Runner(%s) throws unhandled exception", runner->Name().c_str());
                }
            }
        }

        _busy = false;
    }

    static void Discard()
    {
        _init = false;

        // Wait Tick to finish
        while (_busy)
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

        _busy = false;
        _starts = false;
        
        // Discard runners
        for (RunnerBase<void>* runner : _runners)
            runner->Discard();
        _runners.clear();

        // Un hook all functions
        Hook->UnSwapAll();
        Hook->UnDetourAll();
        
        // Shared data
        SharedData->Discard();

        // Free memory
        DELETE_HEAP(Memory);
        DELETE_HEAP(Hook);
        DELETE_HEAP(Runners);
        DELETE_HEAP(SharedData);

        // Logger
        if (_options.UseLogger)
        {
            static auto invalidParameterHandler = [](const wchar_t* expression, const wchar_t* function, const wchar_t* file, const unsigned int line, uintptr_t pReserved)
            {
                //wprintf_s(L"%snFunction:%snFile:%snLine:%un", expression, function, file, line);
            };
            _set_invalid_parameter_handler(invalidParameterHandler);

            int _ = fclose(stdout);
            FreeConsole();
        }
    }
};
