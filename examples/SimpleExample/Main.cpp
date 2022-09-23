#include <thread>
#include <Windows.h>

#include "CleanCheat/CleanCheat.h"
#include "SharedData.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

int main(int argc, char* argv[])
{
    int initData = 1;
    
    // Features
    BasicFeature basic;
    basic.Init(&initData);

    TestFeature test;
    test.Init();

    // Runners
    BasicRunner lvlRunner;
    lvlRunner.RegisterFeature(&basic);
    lvlRunner.RegisterFeature(&test);

    // SharedData
    SharedData sharedData;
    
    // Main
    CleanCheat::Init(&sharedData);
    CleanCheat::RegisterRunner(&lvlRunner);

    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        CleanCheat::Tick(&initData);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Clear, No need to delete features and runners as the stack will take care of that
    CleanCheat::Clear();
    
    return 0;
}
