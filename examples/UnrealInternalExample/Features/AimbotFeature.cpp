#include "pch.h"
#include "CleanCheatUtils.h"
#include "CleanCheat.h"
#include "AimbotFeature.h"

struct BestTargetPlayer
{
    CG::AWW3Character* Player;
    float DistanceFromCenterOfScreen;
    float WorldDistance;
} LastBestTargetPlayer;

bool AimbotFeature::IsValidPlayerState(const CG::AWW3PlayerState* const playerState)
{
    return playerState && playerState->CurrentSquad && playerState->CurrentSquad->Team;
}

bool AimbotFeature::CheckVisibility(const CG::ACharacter* const character, const int32_t boneIndex, CG::FHitResult* outHitResult) const
{
    static auto kSystemLibrary = CG::UObject::FindObject<CG::UKismetSystemLibrary>("KismetMathLibrary Engine.Default__KismetMathLibrary");

    if (!character || !character->Mesh)
        return false;

    static auto const none = CG::FName();

    if (!CleanCheat::SharedData->GController || !CleanCheat::SharedData->GController->PlayerCameraManager)
        return false;

    CG::FVector const startLoc = CleanCheat::SharedData->GController->PlayerCameraManager->GetCameraLocation();
    CG::FVector const endLoc = character->Mesh->GetSocketLocation(character->Mesh->GetBoneName(boneIndex));

    CG::TArray<CG::AActor*> toIgnore{};
    CG::FHitResult hitResult;
    bool lineTrack = kSystemLibrary->STATIC_LineTraceSingle(
        *CG::UWorld::GWorld,
        startLoc,
        endLoc,
        CG::ETraceTypeQuery::TraceTypeQuery1,
        false,
        toIgnore,
        CG::EDrawDebugTrace::None,
        &hitResult,
        true,
        CG::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
        CG::FLinearColor(0, 0, 0, 0),
        0.0f);

    if (lineTrack)
    {
        if (outHitResult)
            *outHitResult = hitResult;

        CG::AActor const* actor = hitResult.Actor.Get();
        if (actor && actor == character)
            return true;
    }

    return false;
}

void AimbotFeature::OnExecute(CG::AActor* curActor)
{
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
        LOG("BONEID: %d", _targetBoneId);
        ++_targetBoneId;
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        LOG("BONEID: %d", _targetBoneId);
        --_targetBoneId;
    }
    
    CleanCheatUtils::DrawCircle(
        CleanCheat::SharedData->CurrentCanvas,
        CleanCheat::SharedData->ScreenCenterPos,
        _radius,
        64,
        _targetCircleColor);

    if (!CleanCheat::SharedData->GCharacter || !_targetPlayer)
        return;

    CG::FVector cameraLocation = CleanCheat::SharedData->GController->PlayerCameraManager->GetCameraLocation();
    CG::FVector targetPlayerHead = _targetPlayer->Mesh->GetBoneWorldPos(_targetBoneId);
    CG::FRotator rotator = (targetPlayerHead - cameraLocation).ToRotator();

    CleanCheat::SharedData->GController->ControlRotation = rotator;
    CleanCheat::SharedData->CurrentCanvas->K2_DrawBox(
        CleanCheatUtils::WorldToScreen(targetPlayerHead),
        {4.f, 4.f},
        1.0f,
        {255.0f, 255.0f, 0.0f, 255.0f});

    auto* player = reinterpret_cast<CG::AWW3Character*>(curActor);
    if (!player || player == CleanCheat::SharedData->GCharacter || player->CurrentHealth <= 0)
        return;

    CG::AWW3PlayerState* playerState = player->SavedPlayerState;
    if (!IsValidPlayerState(playerState))
        return;

    if (playerState->PlayingState != CG::EWW3PlayingState::EWW3PS_Alive)
        return;

    if (playerState->CurrentSquad->Team->TeamId == CleanCheat::SharedData->GCharacter->SavedPlayerState->CurrentSquad->Team->TeamId)
        return;

    // Hold
    if (!(GetAsyncKeyState(VK_RBUTTON) >> 15))
    {
        LastBestTargetPlayer.Player = nullptr;
        return;
    }

    if (_targetPlayer && _targetPlayer->CurrentHealth > 0)
    {
        _targetPlayer = nullptr;
        return;
    }

    if (!player->RootComponent || !player->Mesh)
        return;

    bool w2Screen;
    CG::FVector2D headPos = CleanCheatUtils::WorldToScreen(player->RootComponent->RelativeLocation, &w2Screen);
    
    if (w2Screen && CleanCheatUtils::IsInCircle(headPos, CleanCheat::SharedData->ScreenCenterPos, _radius) && CheckVisibility(player, _targetBoneId))
    {
        const float distance = CleanCheat::SharedData->GCharacter->RootComponent->RelativeLocation.DistanceMeter(player->RootComponent->RelativeLocation);
        const float distanceInScr = std::abs(CleanCheat::SharedData->ScreenCenterPos.Distance(headPos));

        if (!LastBestTargetPlayer.Player || LastBestTargetPlayer.DistanceFromCenterOfScreen > distanceInScr || LastBestTargetPlayer.WorldDistance > distance)
        {
            LastBestTargetPlayer.Player = player;
            LastBestTargetPlayer.WorldDistance = distance;
            LastBestTargetPlayer.DistanceFromCenterOfScreen = distanceInScr;
        }
    }
}

std::string AimbotFeature::Name()
{
    return "AimbotFeature";
}

bool AimbotFeature::Condition(CG::AActor* curActor)
{
    return CleanCheat::SharedData
        && CleanCheat::SharedData->CurrentCanvas
        && CleanCheat::SharedData->GWorld
        && CleanCheat::SharedData->GCharacter
        && curActor->IsA(CG::AWW3Character::StaticClass());
}

void AimbotFeature::AfterExecute()
{
    _targetPlayer = LastBestTargetPlayer.Player;
}
