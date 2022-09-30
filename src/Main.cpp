#include <thread>
#include <Windows.h>

#include "CleanCheat.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

int main(int argc, char* argv[])
{
    LOG("Hello %s users", "CleanCheat");
    int initData = 1;

    // CleanCheat
    CleanCheatOptions options;
    options.UseLogger = true;
    
    CleanCheat::Init(options);
    
    // Features
    BasicFeature basic;
    basic.Init(&initData);

    TestFeature test;
    test.Init();

    // Runners
    BasicRunner basicRunner;
    basicRunner.RegisterFeature(&basic);
    basicRunner.RegisterFeature(&test);
    
    CleanCheat::RegisterRunner(&basicRunner);

    
    std::vector<void*> addr1 = CleanCheat::Memory->PatternScan("48 89 5C 24", 2);
    
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Clean, No need to delete features and runners as the stack will take care of that
    CleanCheat::Clear();
    
    return EXIT_SUCCESS;
}
