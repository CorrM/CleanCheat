#pragma once

#include "CleanCheat/RunnersCollectionBase.h"
#include "MySharedData.h"
#include "Runners/LevelActorsRunner.h"

#define SHARED_DATA_TYPE        MySharedData

class RunnersCollection final : public RunnersCollectionBase
{
public:
    LevelActorsRunner* LevelActors = new LevelActorsRunner();
};
