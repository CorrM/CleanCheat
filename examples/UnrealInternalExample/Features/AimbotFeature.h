#pragma once
#include "CleanCheat/FeatureBase.h"

class CG::AActor;
class CG::AWW3Character;

class AimbotFeature final : public FeatureBase<CG::AActor>
{
private:
    float _radius = 30.0f;
    int _targetBoneId = 55; // 32, 55 => head
    CG::AWW3Character* _targetPlayer = nullptr;
    CG::FLinearColor _targetCircleColor = {255.0f, 255.0f, 255.0f, 255.0f};

private:
    static bool IsValidPlayerState(const CG::AWW3PlayerState* playerState);
    
private:
    bool CheckVisibility(const CG::ACharacter* character, int32_t boneIndex, CG::FHitResult* outHitResult = nullptr) const;

protected:
    void OnExecute(CG::AActor* curActor) override;

public:
    bool Condition(CG::AActor* curActor) override;
    void AfterExecute() override;
};
