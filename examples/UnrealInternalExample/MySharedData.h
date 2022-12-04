#pragma once
#include "CleanCheat/SharedDataBase.h"
#include "SDK/CoreUObject_Structs.h"

class CG::UCanvas;

class MySharedData final : public SharedDataBase<CG::UCanvas>
{
public:
	class CG::UWorld* GWorld = nullptr;
	class CG::AWW3Character* GCharacter = nullptr;
	class CG::AShooterPlayerController* GController = nullptr;
	class CG::UFont* RobotoFont = nullptr;
	class CG::UCanvas* CurrentCanvas = nullptr;
	class CG::UMaterial* LootMat = nullptr;
	CG::FVector2D ScreenSize{};
	CG::FVector2D ScreenCenterPos{};

public:
	/// <summary>
	/// Called once every frame
	/// </summary>
	/// <param name="canvas">PostRender canvas</param>
	void Tick(CG::UCanvas* canvas) override;
};