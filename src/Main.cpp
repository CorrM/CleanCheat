#include <thread>
#include <Windows.h>

#include "CleanCheat.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

int main(int argc, char* argv[])
{
    // CleanCheat
    CleanCheatOptions options;
    options.UseLogger = true;
    
    CleanCheat::Init(options);
    
    // Simple log
    LOG("Hello %s users", "CleanCheat");
    
    // Features
    int initData = 1;
    BasicFeature basic;
    basic.Init(&initData);
    
    TestFeature test;
    test.Init();
    
    // Runners
    BasicRunner basicRunner;
    basicRunner.RegisterFeature(&basic);
    basicRunner.RegisterFeature(&test);
    
    CleanCheat::RegisterRunner(&basicRunner);
    
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Discard, No need to delete features and runners as the stack will take care of that
    CleanCheat::Discard();
    
    return EXIT_SUCCESS;
}
