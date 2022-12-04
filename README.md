# CleanCheat

Game cheat base and clean architecture for your next cheat

<!-- TOC -->
* [CleanCheat](#cleancheat)
  * [Features](#features)
  * [Usage](#usage)
    * [Step1: Adding includes](#step1-adding-includes)
    * [Step2: Initialization](#step2-initialization)
      * [Initialize CleanCheat](#initialize-cleancheat)
      * [Initialize Features](#initialize-features)
      * [Initialize Runners](#initialize-runners)
    * [Step3: Use](#step3-use)
      * [Use Shared data](#use-shared-data)
      * [Use Logger](#use-logger)
      * [Use hooks](#use-hooks)
        * [Hook by swap VMT method address](#hook-by-swap-vmt-method-address)
        * [Hook by detour function](#hook-by-detour-function)
      * [Use memory](#use-memory)
        * [Memory pattern scan](#memory-pattern-scan)
  * [Options](#options)
    * [UseLogger option](#uselogger-option)
  * [Concepts](#concepts)
    * [Shared data](#shared-data)
      * [Shared data basic setup](#shared-data-basic-setup)
    * [Runner and Feature](#runner-and-feature)
      * [Runner](#runner)
        * [Runner life cycle](#runner-life-cycle)
      * [DataProvider](#dataprovider)
      * [Feature](#feature)
        * [Feature life cycle](#feature-life-cycle)
  * [Examples](#examples)
  * [Credits](#credits)
  * [Third-party libraries](#third-party-libraries)
  * [Change Log](#change-log)
        * [0.0.3](#003)
        * [0.0.2](#002)
        * [0.0.1](#001)
<!-- TOC -->

## Features

- Clean architecture
- Force your code to be maintainable and easy to read
- Easy to use, initialize and unload(_dll unload_)
- [Easy global/shared data access](#use-shared-data)
- [Logger](#use-logger) (_Basic one_)
- [Hooks](#use-hooks):
  - Un/Detour
  - Un/VMT swap
- [Memory](#use-memory):
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
In cpp/h files you need to include `CleanCheat.h` **NOT** `CleanCheat/CleanCheat.h`

### Step2: Initialization

#### Initialize CleanCheat

You need to pick your options and pass it to `CleanCheat::Init` function
```c++
CleanCheatOptions options;
options.UseLogger = true;

CleanCheat::Init(options);
```

#### Initialize Features
**Features used in the example**
([BasicFeature](src/Features/BasicFeature.h), [TestFeature](src/Features/TestFeature.h))
```c++
int initData = 1;

BasicFeature basic;
basic.Init(&initData);

TestFeature test;
test.Init();
```

#### Initialize Runners

Features must to be initialized before registers it in any runner
[BasicRunner](src/Runners/BasicRunner.h)

```c++
BasicRunner basicRunner;
basicRunner.RegisterFeature(&basic);
basicRunner.RegisterFeature(&test);

// Register the runner
CleanCheat::RegisterRunner(&basicRunner);
```

Check [examples](#examples) for full initialize

### Step3: Use

You need to include `CleanCheat.h` where ever you want to access `CleanCheat` then you can access what [Features](#Features) it provide,
for sure that's after [Initialize CleanCheat](#initialize-cleancheat)

`CleanCheat` do all it's operation from `CleanCheat::Tick` so it needs to be called from any place that get called at least once per frame:

**MessageHandler**  
```c++
while (!(GetAsyncKeyState(VK_END) & 1))
{
    CleanCheat::Tick(&initData);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

CleanCheat::Discard();
```

**Frame hook**:  
[InternalUnrealExample](#examples) use `PostRender` hooked function to do it.  
You can hook any (DirectX, OpenGL, Vulkan, etc) that called every frame

#### Use Shared data

You can access **shared data** related stuff using `CleanCheat::SharedData`
```c++
CleanCheat::SharedData->ANY_DATA;
```

#### Use Logger

You can log by `LOG` macro that's only work if [UseLogger option](#uselogger-option) are `ture`

```c++
int main(int argc, char* argv[])
{
    // Init CleanCheat
    ...
    
    // Log
    LOG("Hello '%s' users", "CleanCheat");
    return 0;
}
```

**Output**:  
Under the hood it use `std::printf` function with format `[FILE_NAME:FUNC_NAME:CODE_LINE] USER_MESSAGE`
```
[Main.cpp:main:11] Hello 'CleanCheat' users
```

#### Use hooks

You can access **hooking** related stuff using `CleanCheat::Hook`

##### Hook by swap VMT method address

```c++
void* outOrignalMethodAddress = nullptr;
CleanCheat::Hook->SwapVmt(INSTANCE_ADDRESS, METHOD_INDEX, &HOOK_FUNC, &outOrignalMethodAddress);
```

##### Hook by detour function

```c++
void* functionAddress = 0x123456CD;
CleanCheat::Hook->Detour(&functionAddress, &HOOK_FUNC);
```

#### Use memory

You can access **memory** related stuff using `CleanCheat::Memory`

##### Memory pattern scan

```c++
// Look for "48 89 5C 24" in main module, and maximum result 2
std::vector<void*> addrs = CleanCheat::Memory->PatternScan("48 89 5C 24", 2);
```

## Options

Options presented by `CleanCheatOptions` struct that are passed when [initialize CleanCheat](#initialize-cleancheat)

### UseLogger option

Enable console logging by [](#use-logger)

## Concepts

### Shared data

The place where you would store global status and shared data, you could also add functions too.
As every code need it's own collection of shared data you will need to make your own class.

#### Shared data basic setup

1. Make a class that inherits from `SharedDataBase`
2. Include its header in `CleanCheat.h` next to `// Your SharedData class`
3. Edit `SHARED_DATA_TYPE` in `CleanCheat/Macros.h` with your shared data class name (default is `SharedDataStruct`)

Then you can [use shared data](#use-shared-data)

### Runner and Feature

Let's say i have level objects loop that have like 10k object, and every **type** of objects have different task to do,
if i have `PlayerObject` and `WeaponObject`, and i want to make ESP for players and zero recoil for weapons,
then that is a different tasks but related to same task(object iterate).

Problem example (trust me, code will be (very UGLY and very hard to maintain) fast):
```c++
for (int i = 0; i < OBJECT_COUNT; ++i)
{
    if (OBJECT[i] is PlayerObject) // Condetion
    {
        if (!OBJECT[i]->IsDead() && OBJECT[i]->Distance < 100.f) // Condetion
        {
            // Collect/Preper data
            ...
            
            // Draw
            ...
        }
    }
    
    if (OBJECT[i] is WeaponObject) // Condetion
    {
        if (!OBJECT[i]->IsEmpty()) // Condetion
        {
            // Collect/Preper data
            ...
            
            // Handle
            OBJECT[i]->Recoil = 0.f;
        }
    }
}
```

#### Runner

Runner concept present task that can be splited into tasks([Features](#feature)) with providing input to that tasks to check and handle.

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

##### Runner life cycle

| Func       | Description                                                      |
|------------|------------------------------------------------------------------|
| OnExecute  | Called by CleanCheat every tick                                  |
| Condition  | Called before OnExecute by CleanCheat to determine run it or not |
| Discard    | Called by CleanCheat when CleanCheat itself get discarded        |

#### DataProvider

TODO

#### Feature

Feature concept present [runner](#runner) sub-task that can do **one task** with **one input** after pass a **condition**.
So you can't pass same feature to multi runner (unexpected behavior).
Feature are there to spilt runner code and make it easy to maintain easy to read.

In our example feature can be presented as a **Player ESP** task and **Weapon no recoil** task
in separate isolated class like that:
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

##### Feature life cycle

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

## Change Log

##### 0.0.3
- Fix `cannot convert` bug

##### 0.0.2
- Add feature settings

##### 0.0.1
- First release
