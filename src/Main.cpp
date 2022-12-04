#include <thread>
#include <Windows.h>

#include "CleanCheat.h"

#include "DataProviders/BasicDataProvider.h"
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
    LOG("Hello %s users, press %s key to exit", "CleanCheat", "END");

    // DataProviders
    BasicDataProvider basicData;
    basicData.Init();
    
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
    basicRunner.RegisterDataProvider(&basicData);
    
    CleanCheat::RegisterRunner(&basicRunner);
    
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);
        LOG("BasicDataProvider say: %s", basicData.GetData()->c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Discard, No need to delete features, data providers and runners as the stack will take care of that
    CleanCheat::Discard();
    
    return EXIT_SUCCESS;
}
