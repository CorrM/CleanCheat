#include "pch.h"
#include "Utils.h"
#include "CleanCheat/CleanCheat.h"
#include "SharedDataStruct.h"
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

    auto* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();
    if (!character || !character->Mesh)
        return false;

    static auto const none = CG::FName();

    if (!sharedData->GController || !sharedData->GController->PlayerCameraManager)
        return false;

    CG::FVector const startLoc = sharedData->GController->PlayerCameraManager->GetCameraLocation();
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

void AimbotFeature::OnActorLevelLoop(CG::AActor* curActor)
{
    auto* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();
    auto* player = reinterpret_cast<CG::AWW3Character*>(curActor);

    if (!player || player == sharedData->GCharacter || player->CurrentHealth <= 0)
        return;

    CG::AWW3PlayerState* playerState = player->SavedPlayerState;
    if (!IsValidPlayerState(playerState))
        return;

    if (playerState->PlayingState != CG::EWW3PlayingState::EWW3PS_Alive)
        return;
    
    if (playerState->CurrentSquad->Team->TeamId == sharedData->GCharacter->SavedPlayerState->CurrentSquad->Team->TeamId)
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
    CG::FVector2D headPos = Utils::WorldToScreen(player->RootComponent->RelativeLocation, &w2Screen);
    
    if (w2Screen && Utils::IsInCircle(headPos, sharedData->ScreenCenterPos, _radius) && CheckVisibility(player, _targetBoneId))
    {
        const float distance = sharedData->GCharacter->RootComponent->RelativeLocation.DistanceMeter(player->RootComponent->RelativeLocation);
        const float distanceInScr = std::abs(sharedData->ScreenCenterPos.Distance(headPos));
        
        if (!LastBestTargetPlayer.Player || LastBestTargetPlayer.DistanceFromCenterOfScreen > distanceInScr || LastBestTargetPlayer.WorldDistance > distance)
        {
            LastBestTargetPlayer.Player = player;
            LastBestTargetPlayer.WorldDistance = distance;
            LastBestTargetPlayer.DistanceFromCenterOfScreen = distanceInScr;
        }
    }
}

void AimbotFeature::OnExecute(CG::AActor* curActor)
{
    auto* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();

    /*
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        ++_targetBoneId;
    
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        --_targetBoneId;
    */
    // LOG("BONEID: %d", _targetBoneId);
    
    Utils::DrawCircle(
        sharedData->CurrentCanvas,
        sharedData->ScreenCenterPos,
        _radius,
        64,
        _targetCircleColor);
    
    if (!sharedData->GCharacter || !_targetPlayer)
        return;
    
    //if (!(GetAsyncKeyState(VK_MENU) & 1))
    //    return;

    //if (!sharedData->GController->LineOfSightTo(_targetPlayer, CG::FVector(), false))
    //    return;

    //CG::TArray<CG::FTransform>& componentSp = (uintptr_t)_targetPlayer->Mesh + offset;

    constexpr float smoothness = 1.0f;
    CG::FVector cameraLocation = sharedData->GController->PlayerCameraManager->GetCameraLocation();
    CG::FVector targetPlayerHead = _targetPlayer->Mesh->GetBoneWorldPos(_targetBoneId);
    //CG::FVector targetPlayerHead = _targetPlayer->RootComponent->RelativeLocation;
    
    /*
    CG::TArray<CG::AActor*> blankArray{};
    CG::FHitResult hitResult{};
    kSystemLibrary->STATIC_LineTraceSingle(
        reinterpret_cast<CG::UObject*>(CG::UWorld::GWorld),
        cameraLocation,
        targetPlayerHead,
        CG::ETraceTypeQuery::TraceTypeQuery3,
        true,
        blankArray,
        CG::EDrawDebugTrace::ForDuration,
        &hitResult,
        true,
        {1.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        5.0f);
    */
    
    //CG::FVector targetPos = cameraLocation
    //                    + (sharedData->GCharacter->GetVelocity()
    //                    * cameraLocation.Distance(targetPlayerHead)
    //                    / 49650.f
    //                    * smoothness);
    
    CG::FRotator rotator = (targetPlayerHead - cameraLocation).ToRotator();
    
    sharedData->GController->ControlRotation = rotator;
    sharedData->CurrentCanvas->K2_DrawBox(
        Utils::WorldToScreen(targetPlayerHead),
        {4.f, 4.f},
        1.0f,
        {255.0f, 255.0f, 0.0f, 255.0f});
}

bool AimbotFeature::Condition(CG::AActor* curActor)
{
    auto* sharedData = CleanCheat::GetSharedData<SharedDataStruct>();
    return sharedData
        && sharedData->CurrentCanvas
        && sharedData->GWorld
        && sharedData->GCharacter
        && curActor->IsA(CG::AWW3Character::StaticClass());
}

void AimbotFeature::AfterExecute()
{
    _targetPlayer = LastBestTargetPlayer.Player;
}
