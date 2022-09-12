#include "CleanCheat.h"

bool CleanCheat::_init = false;
std::vector<RunnerBase<void>*> CleanCheat::_runners;
SharedDataBase* CleanCheat::_sharedData = nullptr;

void CleanCheat::Init(SharedDataBase* sharedData)
{
    if (_init)
        return;
    _init = true;
    
    _sharedData = sharedData;
}

void CleanCheat::Tick(void* sharedDataParam)
{
    if (!_init)
        return;
    
    _sharedData->Tick(sharedDataParam);
    
    for (RunnerBase<void>*& runner : _runners)
    {
        if (runner->Condition())
            runner->Tick();
    }
}

void CleanCheat::Clear()
{
    _runners.clear();
}
