#include "pch.h"
#include "Global.h"
#include "Utils.h"
#include "Menu.h"
#include "SharedDataStruct.h"

#include "CleanCheat/CleanCheat.h"

#include "Runners/LevelActorsRunner.h"

#include "Features/AimbotFeature.h"
#include "Features/ChamsFeature.h"

typedef void(__thiscall* ProcessEventType)(CG::UObject*, CG::UFunction*, void*);
typedef void(__thiscall* PostRenderType)(CG::UGameViewportClient*, CG::UCanvas*);

#if CONSOLE_OUTPUT
FILE* ConsoleCOut = nullptr;
#endif

bool Unload = false;
bool Once = false;
bool Working = false;

ProcessEventType OProcessEvent = nullptr;
PostRenderType OPostRender = nullptr;

void DllUnload();

void WINAPI ProcessEventHook(CG::UObject* thiz, CG::UFunction* function, void* parms)
{
	//LOG("%s", function->GetFullName().c_str());
	OProcessEvent(thiz, function, parms);
}

void WINAPI PostRenderHook(CG::UGameViewportClient* gameViewportClient, CG::UCanvas* canvas)
{
	auto* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();

	try
	{
		if (!Once)
		{
			Once = true;

			CleanCheat::DetourFunction(reinterpret_cast<void**>(&OProcessEvent), reinterpret_cast<void*>(&ProcessEventHook));
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
	if (!sharedData)
		goto Exit;

	try
	{
		CleanCheat::Tick(canvas);
	}
	catch (...) {}
	
	// Menu
	if (sharedData)
		Menu::Tick(canvas, sharedData->RobotoFont);

	Exit:
	OPostRender(gameViewportClient, canvas);
}

void MainEntryPoint(HMODULE hModule)
{
	if (!CG::InitSdk())
	{
		LOG("SDK initialization failed");
		return;
	}
	LOG("SDK Initialized successfully");
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
	LOG("ViewportClient : 0x%llx", reinterpret_cast<uintptr_t>(localPlayer->ViewportClient));

	// Features
	auto* chams = new ChamsFeature();
	chams->Init();
	
	auto* aimbot = new AimbotFeature();
	aimbot->Init();

	// Runners
	auto* actorRunner = new LevelActorsRunner();
	
	bool runnerInit = true;
	runnerInit &= actorRunner->RegisterFeature(chams);
	runnerInit &= actorRunner->RegisterFeature(aimbot);

	if (!runnerInit)
	{
		LOG("Runner failed");
		return;
	}
	
	// SharedData
	auto* sharedData = new SharedDataStruct();

	// Init CleanCheat
	CleanCheat::Init(sharedData);
	CleanCheat::RegisterRunner(actorRunner);

	// PostRender
	std::vector<CG::UGameViewportClient*> gameViewportClients = CG::UObject::FindObjects<CG::UGameViewportClient>();
	LOG("GameViewportClientCount: %d", static_cast<int>(gameViewportClients.size()));
	for (int i = 1; i < static_cast<int>(gameViewportClients.size()) - 1; ++i)
	{
		CG::UGameViewportClient*& gameViewportClient = gameViewportClients[i];
		
		void** gameViewportClientVmt = *reinterpret_cast<void***>(gameViewportClient);
		LOG("PostRender     : 0x%llx", reinterpret_cast<uintptr_t>(gameViewportClientVmt[POST_RENDER_INDEX]));

		CleanCheat::SwapVmtFunction(gameViewportClient, POST_RENDER_INDEX, &PostRenderHook, reinterpret_cast<void**>(&OPostRender));
	}

	// ProcessEventType
	void** localPlayerVmt = *reinterpret_cast<void***>(localPlayer);
	OProcessEvent = reinterpret_cast<ProcessEventType>(localPlayerVmt[PROCESS_EVENT_INDEX]);
	LOG("ProcessEvents  : 0x%llx", reinterpret_cast<uintptr_t>(OProcessEvent));
}

void DllUnload()
{
	Unload = true;

	if (OPostRender)
	{
		CG::ULocalPlayer* localPlayer = Utils::GetLocalPlayer();
		CleanCheat::UnSwapVmtFunction(localPlayer->ViewportClient, POST_RENDER_INDEX, &OPostRender);
	}
	
	CleanCheat::Clear();

#if CONSOLE_OUTPUT
	fclose(ConsoleCOut);
	//fclose(stdout);
	FreeConsole();
#endif
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ulReasonForCall, LPVOID lpReserved)
{
	if (ulReasonForCall != DLL_PROCESS_ATTACH)
		return TRUE;

#if CONSOLE_OUTPUT
	AllocConsole();
	freopen_s(&ConsoleCOut, "CONOUT$", "w", stdout);
#endif

	bool error = false;
	try
	{
		MainEntryPoint(hModule);
	}
	catch (const std::runtime_error& e)
	{
#if CONSOLE_OUTPUT
		LOG("Error: %s", e.what());
#endif
		
		error = true;
	}
	catch (const std::exception& e)
	{
#if CONSOLE_OUTPUT
		LOG("Error: %s", e.what());
#endif
		
		error = true;
	}
	catch (const std::string& ex)
	{
#if CONSOLE_OUTPUT
		LOG("Error: %s", ex.c_str());
#endif
		
		error = true;
	}
	catch (...)
	{
#if CONSOLE_OUTPUT
		LOG("Error!!!!!!!");
#endif
		
		error = true;
	}

	if (error)
	{
#if CONSOLE_OUTPUT
		Sleep(5 * 1000);
#endif
		DllUnload();
	}
	
	return TRUE;
}

