#pragma once

#include "MySharedData.h" // Your SharedData class
#include "CleanCheat/RunnersCollectionBase.h"
#include "Runners/BasicRunner.h"

// SETTINGS
#define USE_LOGGER              1
#define SHARED_DATA_TYPE        MySharedData

// Runners
class RunnersCollection final : RunnersCollectionBase
{
public:
    BasicRunner* Basic = new BasicRunner();
};
