#include "pch.h"
#include "Global.h"
#include "Utils.h"

#include "CleanCheat.h"

#define GAME_VIEW_PORT_INDEX 0x02  // Maybe you need to change that number

typedef void (__thiscall* ProcessEventType)(CG::UObject*, CG::UFunction*, void*);
typedef void (__thiscall* PostRenderType)(CG::UGameViewportClient*, CG::UCanvas*);

ProcessEventType OProcessEvent = nullptr;
PostRenderType OPostRender = nullptr;

bool Unload = false;
bool Once = false;
bool Working = false;

bool InitCleanCheat()
{
    // Init CleanCheat
    CleanCheatOptions options;
    options.UseLogger = true;

    CleanCheat::Init(options);

    LevelActorsRunner* lvlRunner = CleanCheat::Runners->LevelActors;

    // Features
    bool featuresInit = true;
    featuresInit &= lvlRunner->Features->Chams->Init();
    if (!featuresInit)
    {
        LOG("Features initialize failed");
        return false;
    }

    if (!lvlRunner->Init())
    {
        LOG("Runner registration failed");
        return false;
    }

    return CleanCheat::Start();
}

void DllUnload()
{
    Unload = true;
    CleanCheat::Discard();
}

void __stdcall ProcessEventHook(CG::UObject* thiz, CG::UFunction* function, void* parms)
{
    //LOG("%s", function->GetFullName().c_str());
    OProcessEvent(thiz, function, parms);
}

void __stdcall PostRenderHook(CG::UGameViewportClient* gameViewportClient, CG::UCanvas* canvas)
{
    // Hook ProcessEvent from game thread, otherwise some games will crash
    try
    {
        if (!Once)
        {
            Once = true;

            CleanCheat::Hook->Detour(reinterpret_cast<void**>(&OProcessEvent), reinterpret_cast<void*>(&ProcessEventHook));
            LOG("Hook 'ProcessEvent' ... Success");
        }
    }
    catch (...)
    {
        LOG("Error: %s", "Hook 'ProcessEvent' falied");
    }

    // Unload
    if (GetAsyncKeyState(UNLOAD_KEY) & 1)
    {
        Unload = true;
        DllUnload();
        goto Exit;
    }

    if (Unload)
        goto Exit;

    // Shared data
    if (!CleanCheat::SharedData)
        goto Exit;

    //canvas->K2_DrawLine({0.0, 0.0}, {50.0, 50.0}, 1.f, {1.f, 1.f, 1.f, 1.f});
    CleanCheat::Tick(canvas);

Exit:
    OPostRender(gameViewportClient, canvas);
}

void MainEntryPoint(HMODULE hModule)
{
    // SDK
    if (!CG::InitSdk())
    {
        MessageBox(nullptr, TEXT("SDK initialization failed"), TEXT("Error"), MB_OK);
        return;
    }
    LOG("SDK Initialized successfully");
    
    // CleanCheat
    if (!InitCleanCheat())
    {
        LOG("CleanCheat initialization failed");
        return;
    }
    LOG("ModuleBase: %p", static_cast<void*>(GetModuleHandleA(nullptr)));

    // GWorld
    CG::UWorld* gWorld = *CG::UWorld::GWorld;
    if (!gWorld)
    {
        LOG("GWorld is nullptr");
        return;
    }
    LOG("GWorld: %p", gWorld);

    // LocalPlayer
    CG::ULocalPlayer* localPlayer = Utils::GetLocalPlayer();
    if (!localPlayer)
    {
        LOG("localPlayer is nullptr");
        return;
    }
    LOG("LocalPlayer: %p", localPlayer);
    
    // PostRender
    LOG("ViewportClient : 0x%llx", reinterpret_cast<uintptr_t>(localPlayer->ViewportClient));
    std::vector<CG::UGameViewportClient*> gameViewportClients = CG::UObject::FindObjects<CG::UGameViewportClient>();
    LOG("GameViewportClientCount: %d", static_cast<int>(gameViewportClients.size()));

    CG::UGameViewportClient*& gameViewportClient = gameViewportClients[GAME_VIEW_PORT_INDEX];
    void** gameViewportClientVmt = *reinterpret_cast<void***>(gameViewportClient);
    LOG("PostRender     : 0x%llx", reinterpret_cast<uintptr_t>(gameViewportClientVmt[POST_RENDER_INDEX]));

    CleanCheat::Hook->SwapVmt(
        gameViewportClient,
        POST_RENDER_INDEX,
        reinterpret_cast<void*>(&PostRenderHook),
        reinterpret_cast<void**>(&OPostRender));

    // ProcessEvent
    void** localPlayerVmt = *reinterpret_cast<void***>(localPlayer);
    OProcessEvent = reinterpret_cast<ProcessEventType>(localPlayerVmt[PROCESS_EVENT_INDEX]);
    LOG("ProcessEvents  : 0x%llx", reinterpret_cast<uintptr_t>(OProcessEvent));
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ulReasonForCall, LPVOID lpReserved)
{
    if (ulReasonForCall != DLL_PROCESS_ATTACH)
        return TRUE;

    bool error = false;
    try
    {
        MainEntryPoint(hModule);
    }
    catch (const std::exception& e)
    {
        LOG("Error: %s", e.what());
        error = true;
    }
    catch (const std::string& ex)
    {
        LOG("Error: %s", ex.c_str());
        error = true;
    }
    catch (...)
    {
        LOG("Error!!!!!!!");
        error = true;
    }

    if (error)
    {
        constexpr int sleep = 5 * 1000;
        Sleep(sleep);
        DllUnload();
    }

    return TRUE;
}
