# CleanCheat

Game cheat base and clean architecture for your next cheat, even it's a cheat it should be clean

## Why CleanCheat

**Project maintenance**

Let's say i have level objects loop that have like 10k object, and every **type** of objects have different task to do,
if i have `PlayerObject` and `WeaponObject`, and i want to make ESP for players and zero recoil for weapons,
then that is a different tasks but related to same task(object iterate).

Problem example code will be very UGLY and very hard to maintain fast:
```c++
for (int i = 0; i < OBJECT_COUNT; ++i)
{
    if (OBJECT[i] is PlayerObject) // Condition
    {
        if (!OBJECT[i]->IsDead() && OBJECT[i]->Distance < 100.f) // Condition
        {
            // Collect/Prepare data
            ...
            
            // Draw
            ...
        }
    }
    
    if (OBJECT[i] is WeaponObject) // Condition
    {
        if (!OBJECT[i]->IsEmpty()) // Condition
        {
            // Collect/Prepare data
            ...
            
            // Handle
            OBJECT[i]->Recoil = 0.f;
        }
    }
}
```

**Reusability**

With some tweaks you can write Runners, DataProviders and Features only once and share them in all your projects

## Table Of Content

<!-- TOC -->
* [CleanCheat](#cleancheat)
  * [Why CleanCheat](#why-cleancheat)
  * [Table Of Content](#table-of-content)
  * [Features](#features)
  * [Usage](#usage)
    * [Step1: Adding includes](#step1--adding-includes)
    * [Step2: Initialization](#step2--initialization)
    * [Step3: Use](#step3--use)
  * [Options](#options)
  * [Concepts](#concepts)
    * [Shared data](#shared-data)
    * [Runner](#runner)
    * [Runner Task](#runner-task)
      * [DataProvider](#dataprovider)
      * [Feature](#feature)
  * [Examples](#examples)
  * [Contributors](#contributors)
  * [Third-party libraries](#third-party-libraries)
<!-- TOC -->

## Features

- Clean architecture
- Force your code to be maintainable and easy to read
- Easy to use, initialize and unload(_dll unload_)
- [Easy global/shared data access](#shared-data)
- Logger (_Basic one_)
- Hooks:
  - Un/Detour
  - Un/VMT swap
- Memory:
  - Pattern scan
  - Value scan (`TODO`)

## Usage

To use `CleanCheat` you need to do 3 simple steps

### Step1: Adding includes

At first your project need to know about `CleanCheat`.

**First option**:

Copy `src/CleanCheat` and `src/CleanCheat.h` OR download [Latest Release](https://github.com/CorrM/CleanCheat/releases/latest) to your project dir

**Second option**:

Copy CleanCheat repo or add as git submodule (`git submodule add https://github.com/CorrM/CleanCheat.git CleanCheat`)

Edit `.vcxproj` file:

Add this to ItemGroup that have `ClCompile` tags ([Exmaple](https://github.com/CorrM/CleanCheat/blob/main/examples/UnrealInternalExample/UnrealInternalExample.vcxproj#L176-L178))
```
<ClCompile Include=".\CleanCheat\src\CleanCheat\**\*.cpp">
    <PrecompiledHeader>NotUsing</PrecompiledHeader>
</ClCompile>
```

Add this to ItemGroup that have `ClInclude` tags ([Example](https://github.com/CorrM/CleanCheat/blob/main/examples/UnrealInternalExample/UnrealInternalExample.vcxproj#L198))
```
<ClInclude Include=".\CleanCheat\src\CleanCheat\**\*.h" />
```

**Now include CleanCheat in your project**

- Add `#include "CleanCheat.h"` into your cpp/h files (if you have precompiled headers it is a good place to add this include there)
- [Initialize shared data](#shared-data-basic-setup) if you will use it

**Note**:  
In cpp/h files you need to include `CleanCheat.h` **NOT** `CleanCheat/CleanCheatManager.h`

### Step2: Initialization

**CleanCheatSettings**  

Create `CleanCheatSettings.h` file in your project folder and its content should be like that

```c++
#pragma once

#include "CleanCheat/RunnersCollectionBase.h"
#include "SHARED_DATA_FILENAME.h"
#include "Runners/BasicRunner.h"

#define SHARED_DATA_TYPE        SHARED_DATA_CLASS_NAME

class RunnersCollection final : public RunnersCollectionBase
{
public:
    // RUNNERS
};
```

This file is the only file you need to edit to start using CleanCheat, so that are things you need to change:

| Name                   | Description                  |
|------------------------|------------------------------|
| SHARED_DATA_FILENAME   | Shared data header file name |
| SHARED_DATA_CLASS_NAME | Shared data class name       |
| RUNNERS                | Runners you want to register |

_Example [CleanCheatSettings.h](examples/BasicExample/CleanCheatSettings.h)_  
_RUNNERS must to be `pointer to runner`, any other field type will lead to unexpected behavior_

**Initialize CleanCheat**  

First thing to do after create `CleanCheatSettings.h` is initialize `CleanCheat` with [Options](#options) you prefer, actually this should be
first code you be executed in your project, this is how to initialize `CleanCheat`:

```c++
CleanCheatOptions options;
options.UseLogger = true;
options.ConsoleTitle = L"CleanCheat";

if (!CleanCheat::Init(options))
    CleanCheat::Discard();
```

**Initialize Runners**

To initialize [Runner](#runner) you need first to initialize all [DataProviders](#dataprovider) and [Features](#feature) it hold:  
_(Prefer to initialize [DataProviders](#dataprovider) first)_

```c++
// # BasicRunner
// Get basic runner
BasicRunner* basicRunner = CleanCheat::Runners->Basic;
BasicRunnerDataProviders* basicDataProviders = basicRunner->DataProviders;
BasicRunnerFeatures* basicDataFeatures = basicRunner->Features;

// # DataProviders
// Get BasicDataProvider and initalize
basicDataProviders->Basic->Init();

// # Features
// Get BasicFeature and initalize
basicDataFeatures->Basic->Init(&initData);
// Get TestFeature and initalize
basicDataFeatures->Test->Init();

// Init runner after its tasks
basicRunner->Init();

// Other runners
// ...
```
_([BasicRunner](examples/BasicExample/Runners/BasicRunner.h))_

**Start**

After initialize runners your need to start `CleanCheat`:

```c++
CleanCheat::Start();
```

**Execute**

`CleanCheat` do all it's operation by `CleanCheat::Tick` so it needs to be called from any where that get called at least once per frame:
You can hook any (DirectX, OpenGL, Vulkan, etc) that called every frame
[InternalUnrealExample](#examples) use `PostRender` hooked function to do it.  

```c++
int sharedDataTickParam = 0;
CleanCheat::Tick(&sharedDataTickParam);
```

**Discard**

Discard, stop and clean every operation done by CleanCheat, should only called once before exit.  
_(that should give you ability to unload your internal dll without need to restart or kill current process)_

```c++
CleanCheat::Discard();
```

### Step3: Use

**Use Shared data**

You can access **shared data** related stuff using `CleanCheat::SharedData`
```c++
// Access shared data
CleanCheat::SharedData->ANY_DATA;
```

**Logger**

You can log by `LOG` macro that's only work if `UseLogger` option are `ture`

```c++
int main(int argc, char* argv[])
{
    // Init CleanCheat
    ...
    
    // Log
    LOG("Hello '%s' users", "CleanCheat");
    return 0;
}

// Output
// [Main.cpp:main:11] Hello 'CleanCheat' users
```

**Hooks**

You can access **hooking** related stuff using `CleanCheat::Hook`

```c++
// Swap VMT method address
void* outOrignalMethodAddress = nullptr;
CleanCheat::Hook->SwapVmt(INSTANCE_ADDRESS, METHOD_INDEX, &HOOK_FUNC, &outOrignalMethodAddress);

// Detour function
void* functionAddress = 0x123456CD;
CleanCheat::Hook->Detour(&functionAddress, &HOOK_FUNC);
```

**Memory**

You can access **memory** related stuff using `CleanCheat::Memory`

```c++
// Pattern scan
// Look for "48 89 5C 24" in main module, and maximum result 2 (0 for get all result)
std::vector<void*> addrs = CleanCheat::Memory->PatternScan("48 89 5C 24", 2);
```

## Options

Options presented by `CleanCheatOptions` struct that are passed when [initialize CleanCheat](#step2--initialization)

| Option       | Type   | Description                                                    |
|--------------|--------|----------------------------------------------------------------|
| UseLogger    | bool   | Enable console logging by attaching or allocate console window |
| ConsoleTitle | string | Console window title                                           |

## Concepts

### Shared data

The place where you would store global status and shared data, you could also add functions too.
As every code need it's own collection of shared data you will need to make your own class.

**Shared data basic setup**

1. Follow [CleanCheatSettings Initialization](#step2--initialization)
2. Make a class that inherits from `SharedDataBase`
3. Include its header in `CleanCheatSettings.h`
4. Edit `SHARED_DATA_TYPE` in `CleanCheatSettings.h` with your shared data class name

### Runner

Runner concept present task that can be split into tasks([Features](#feature)) with providing input to that tasks to check and handle.

In our example runner will present the loop `for (int i = 0; i < OBJECT_COUNT; ++i)` and iterator `Object` as input for our [Features](#feature).
```c++
bool LevelObjectsRunner::Condition()
{
    return CleanCheat::SharedData && CleanCheat::SharedData->World;
}

void LevelObjectsRunner::OnExecute()
{
    for (int32_t i = 0; i < curLevel->Objects.Count(); i++)
    {
        auto* curObject = curLevel->Objects[i];
        if (!curObject)
            continue;

        // Here we execute all registered features with 'curObject' as input
        ExecuteFeatures(curObject);
    }
}
```

**Runner life cycle**

| Func       | Description                                                      |
|------------|------------------------------------------------------------------|
| OnExecute  | Called by CleanCheat every tick                                  |
| Condition  | Called before OnExecute by CleanCheat to determine run it or not |
| Discard    | Called by CleanCheat when CleanCheat itself get discarded        |

### Runner Task

Runner task is the concept of any task that run under [Runner](#runner) wings.

| Type         | Description                                                             |
|--------------|-------------------------------------------------------------------------|
| DataProvider | Task that exist to provide data to runner itself or one of its features |
| Feature      | Task runner will invoke to handle part of your project logic            |

#### DataProvider

DataProvider concept present [Runner](#runner) sub-task that can do calculate/grab data with **one input** after pass a **condition**.

```c++
bool BestTargetDataProvider::Condition(Actor* curActor)
{
    return CleanCheat::SharedData
        && CleanCheat::SharedData->CurrentCanvas
        && CleanCheat::SharedData->CurrentWorld
        && CleanCheat::SharedData->GCharacter
        && curActor->IsA(ACharacterBP_C);
}

void BestTargetDataProvider::AfterExecute()
{
    SetData(LastBestTargetPlayer.Player);
}

void BestTargetDataProvider::OnExecute(AActor* curActor)
{
    // ...
}

```

#### Feature

Feature concept present [Runner](#runner) sub-task that can do **one thing** with **one input** after pass a **condition**.
So you can't pass same feature to multi runner (unexpected behavior).
Feature are there to spilt runner code and make it easy to maintain easy to read.

In our example feature can be presented as a **Player ESP** drawer or **Weapon** no recoil
in separate isolated classes like that:
```c++
bool EspFeature::Condition(Object* curObject)
{
    return curObject is PlayerObject && !curObject->IsDead() && curObject->Distance < 100.f;
}

void EspFeature::OnExecute(Object* curObject)
{
    // Collect/Preper data
    ...
            
    // Draw
    ...
}
```

```c++
bool WeaponZeroRecoilFeature::Condition(Object* curObject)
{
    return curObject is WeaponObject && !curObject->IsEmpty();
}

void WeaponZeroRecoilFeature::OnExecute(Object* curObject)
{
    // Collect/Preper data
    ...
            
    // Handle
    curObject->Recoil = 0.f;
}
```

**Feature life cycle**

| Func          | Description                                                                                    |
|---------------|------------------------------------------------------------------------------------------------|
| OnInit        | Called only once by user before it get registers by runner                                     |
| OnExecute     | Called by runner during runner executive once or more depend on the runner                     |
| Condition     | Called before `OnExecute` by runner to determine whether it will call it or not                |
| BeforeExecute | Called by runner **before** call `OnExecute` always get called, doesn't care about `Condition` |
| AfterExecute  | Called by runner **after** call `OnExecute` always get called, doesn't care about `Condition`  |
| Discard       | Called by runner when runner itself get discarded                                              |

## Examples

There are a number of examples that demonstrate various aspects of using `CleanCheat`. They can be found in the examples folder:

| Example                                                            | Description                                                    |
|--------------------------------------------------------------------|----------------------------------------------------------------|
| [Simple](src/Main.cpp)                                             | Simple example                                                 |
| [InternalUnrealEngine](examples/UnrealInternalExample/dllmain.cpp) | Shows how to use with [CheatGear](https://github.com/CorrM/CG) |


## Contributors

<a href="https://github.com/CorrM/CleanCheat/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=CorrM/CleanCheat" />
</a>

## Third-party libraries

[Detours](https://github.com/microsoft/Detours)