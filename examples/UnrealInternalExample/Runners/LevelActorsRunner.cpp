#include "pch.h"
#include "CleanCheat/CleanCheat.h"
#include "../SharedDataStruct.h"
#include "LevelActorsRunner.h"

void LevelActorsRunner::OnExecute()
{
    SharedDataStruct* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();

    CG::TArray<CG::ULevel*>& levels = sharedData->GWorld->levels;
    for (int j = 0; j < levels.Count(); j++)
    {
        if (!levels.IsValidIndex(j))
            continue;

        CG::ULevel* currentLevel = levels[j];
        if (!currentLevel)
            continue;

        for (int32_t i = 0; i < currentLevel->ActorList.Count(); i++)
        {
            CG::AActor* curActor = currentLevel->ActorList[i];
            if (!curActor)
                continue;

            ExecuteFeatures(curActor);
        }
    }
}

bool LevelActorsRunner::Condition()
{
    SharedDataStruct* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();
    return sharedData && sharedData->GWorld;
}
