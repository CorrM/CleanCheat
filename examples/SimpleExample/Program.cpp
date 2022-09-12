#include <thread>

#include "CleanCheat/CleanCheat.h"
#include "SharedData.h"
#include "Features/BasicFeature.h"
#include "Features/TestFeature.h"
#include "Runners/BasicRunner.h"

int main(int argc, char* argv[])
{
    int initData = 1;

    // SharedData
    SharedData sharedData;
    
    // Features
    BasicFeature basic;
    basic.Init(&initData);

    TestFeature test;
    test.Init();

    // Runners
    BasicRunner lvlRunner;
    lvlRunner.RegisterFeature(&basic);
    lvlRunner.RegisterFeature(&test);

    // Main
    CleanCheat::Init(&sharedData);
    CleanCheat::RegisterRunner(&lvlRunner);

    while (true)
    {
        CleanCheat::Tick(&initData);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    return 0;
}
