#include <thread>
#include <Windows.h>

#include "CleanCheat.h"

#include "DataProviders/BasicDataProvider.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

bool CleanCheatInitRunners()
{
    // BasicRunner
    BasicRunner* basicRunner = CleanCheat::Runners->Basic;
    BasicRunnerDataProviders* basicDataProviders = basicRunner->DataProviders;
    BasicRunnerFeatures* basicDataFeatures = basicRunner->Features;
    
    // DataProviders
    bool initChecker = true;
    initChecker &= basicDataProviders->Basic->Init();
    if (!initChecker)
    {
        LOG("ERROR: Can't initialize DataProviders");
        return false;
    }
    
    // Features
    int initData = 1;
    initChecker &= basicDataFeatures->Basic->Init(&initData);
    initChecker &= basicDataFeatures->Test->Init();
    if (!initChecker)
    {
        LOG("ERROR: Can't initialize Features");
        return false;
    }
    
    // Init runner after its tasks
    initChecker &= basicRunner->Init();

    if (!initChecker)
    {
        LOG("ERROR: Can't initialize Runners");
        return false;
    }

    // Other runners
    // ...
    
    return true;
}

int main(int argc, char* argv[])
{
    // CleanCheat
    CleanCheatOptions options;
    options.UseLogger = true;
    options.ConsoleTitle = L"CleanCheat";

    if (!CleanCheat::Init(options))
    {
        LOG("ERROR: Can't initialize CleanCheat");
        CleanCheat::Discard();
        return EXIT_FAILURE;
    }

    if (!CleanCheatInitRunners())
        return EXIT_FAILURE;
    
    CleanCheat::Start();
    
    // # Simple log
    LOG("Hello %s users, press %s key to exit", "CleanCheat", "END");

    int initData = 1;
    BasicRunner* basicRunner = CleanCheat::Runners->Basic;
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);

        const char* dataStr = basicRunner->DataProviders->Basic->GetData()->c_str();
        LOG("BasicDataProvider say: %s", dataStr);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Discard, No need to delete runners as the stack will take care of that
    CleanCheat::Discard();
    
    return EXIT_SUCCESS;
}
