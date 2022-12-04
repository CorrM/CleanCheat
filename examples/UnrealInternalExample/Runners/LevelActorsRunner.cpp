#include "pch.h"
#include "CleanCheat.h"
#include "LevelActorsRunner.h"

void LevelActorsRunner::OnExecute()
{
    CG::TArray<CG::ULevel*>& levels = CleanCheat::SharedData->GWorld->levels;
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

            ExecuteTasks(curActor);
        }
    }
}

std::string LevelActorsRunner::Name()
{
    return "LevelActorsRunner";
}

bool LevelActorsRunner::Condition()
{
    return CleanCheat::SharedData && CleanCheat::SharedData->GWorld;
}
