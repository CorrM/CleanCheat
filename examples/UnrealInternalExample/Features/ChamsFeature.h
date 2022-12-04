#pragma once
#include <vector>
#include "CleanCheat/FeatureBase.h"
#include "../SDK/CoreUObject_Structs.h"

class CG::UCanvas;
class CG::AActor;
class CG::AWW3Character;

class ChamsFeature final : public FeatureBase<CG::AActor>
{
private:
	float _moreInfoRadius = 30.0f;
	std::vector<std::vector<int>> _boneVector;
	CG::FLinearColor _yellowColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	CG::FLinearColor _greenColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	CG::FLinearColor _redColor = { 1.0f, 0.0f, 0.0f, 1.0f };

private:
	static wchar_t* S2Wc(const char* c);
	static CG::FLinearColor* GetPlayerDrawColor(CG::AWW3Character* player);
	static void SetLootMaterial(CG::AActor* actor);
	static bool IsValidPlayerState(const CG::AWW3PlayerState* playerState);

	void PlayerESP(CG::AWW3Character* player) const;

protected:
	bool OnInitialize(void* initData) override;
	void OnExecute(CG::AActor* curActor) override;

public:
	std::string Name() override;
	bool Condition(CG::AActor* curActor) override;
	void BeforeExecute() override;
};
