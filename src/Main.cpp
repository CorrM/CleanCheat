#include <thread>
#include <Windows.h>

#include "CleanCheat.h"

#include "DataProviders/BasicDataProvider.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

int main(int argc, char* argv[])
{
    // # CleanCheat
    CleanCheatOptions options;
    options.UseLogger = true;
    
    CleanCheat::Init(options);

    // # Basic runner
    BasicRunner* basicRunner = CleanCheat::Runners->Basic;
    
    // DataProviders
    basicRunner->DataProviders->Basic->Init();
    
    // Features
    int initData = 1;
    basicRunner->Features->Basic->Init(&initData);
    basicRunner->Features->Test->Init();

    // Init runner after its tasks
    basicRunner->Init();
    
    // # Simple log
    LOG("Hello %s users, press %s key to exit", "CleanCheat", "END");

    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);

        const char* dataStr = basicRunner->DataProviders->Basic->GetData()->c_str()
        LOG("BasicDataProvider say: %s", dataStr);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Discard, No need to delete runners as the stack will take care of that
    CleanCheat::Discard();
    
    return EXIT_SUCCESS;
}
