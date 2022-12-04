#pragma once

#include "CleanCheat/RunnersCollectionBase.h"
#include "MySharedData.h" // Your SharedData class
#include "Runners/BasicRunner.h"

// SETTINGS
#define SHARED_DATA_TYPE        MySharedData

// Runners
class RunnersCollection final : RunnersCollectionBase
{
public:
    BasicRunner* Basic = new BasicRunner();
};
