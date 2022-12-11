#include "pch.h"
#include <cmath>
#include "CleanCheat.h"
#include "../Utils.h"
#include "ChamsFeature.h"

int HKeyTestYMulti = 0;

wchar_t* ChamsFeature::S2Wc(const char* c)
{
    const size_t cSize = strlen(c) + 1;
    auto* wc = new wchar_t[cSize];

    size_t outSize;
    mbstowcs_s(&outSize, wc, cSize, c, cSize - 1);

    return wc;
}

CG::FLinearColor* ChamsFeature::GetPlayerDrawColor(CG::AWW3Character* player)
{
    static CG::FLinearColor safeColor = {0.780f, 0.137f, 0.721f, 1.0f};
    static CG::FLinearColor dangerColor = {0.6f, 0.0f, 0.0f, 1.0f};

    if (!CleanCheat::SharedData->GController)
        return nullptr;

    if (CleanCheat::SharedData->GController->LineOfSightTo(player, CG::FVector(), false))
        return &dangerColor;

    return &safeColor;
}

void ChamsFeature::SetLootMaterial(CG::AActor* const actor)
{
    if (!CleanCheat::SharedData)
        return;

    if (!actor || !CleanCheat::SharedData->LootMat)
        return;

    CG::TArray<CG::UActorComponent*> components = actor->GetComponentsByClass(CG::USkeletalMeshComponent::StaticClass());
    for (int compIdx = 0; compIdx < components.Count(); ++compIdx)
    {
        CG::UActorComponent* const component = components[compIdx];
        if (!component)
            continue;

        auto* const skeletalMeshComp = reinterpret_cast<CG::USkeletalMeshComponent*>(component);
        try
        {
            CG::TArray<CG::UMaterialInterface*> materials = skeletalMeshComp->GetMaterials();
            for (int matIdx = 0; matIdx < materials.Count(); matIdx++)
            {
                if (!materials.IsValidIndex(matIdx))
                    continue;

                if (CleanCheat::SharedData->LootMat && materials[matIdx] != CleanCheat::SharedData->LootMat)
                {
                    if (!skeletalMeshComp)
                        continue;

                    skeletalMeshComp->SetMaterial(matIdx, CleanCheat::SharedData->LootMat);
                }
            }
        }
        catch (...) {}
    }
}

bool ChamsFeature::IsValidPlayerState(const CG::AWW3PlayerState* const playerState)
{
    return playerState && playerState->CurrentSquad && playerState->CurrentSquad->Team;
}

void ChamsFeature::PlayerESP(CG::AWW3Character* const player) const
{
    if (!player || player == CleanCheat::SharedData->GCharacter || player->CurrentHealth <= 0)
        return;

    CG::AWW3PlayerState* playerState = player->SavedPlayerState;
    if (!IsValidPlayerState(playerState))
        return;

    if (playerState->PlayingState != CG::EWW3PlayingState::EWW3PS_Alive)
        return;

    if (playerState->CurrentSquad->Team->TeamId == CleanCheat::SharedData->GCharacter->SavedPlayerState->CurrentSquad->Team->TeamId)
        return;

    bool w2ScreenSuccess;
    CG::FVector2D rootPos = Utils::WorldToScreen(player->RootComponent->RelativeLocation, &w2ScreenSuccess);
    if (!w2ScreenSuccess)
        return;

    CG::FLinearColor* playerDrawColor = GetPlayerDrawColor(player);
    const float distance = CleanCheat::SharedData->GCharacter->RootComponent->RelativeLocation.DistanceMeter(player->RootComponent->RelativeLocation);

    CG::FVector footBonePos = player->Mesh->GetBoneWorldPos(static_cast<int>(CG::EBone_SKM_CharacterMesh_01_01::b_Spine));
    footBonePos.Z -= 130.f;
    CG::FVector2D textStartPos = {rootPos.X, Utils::WorldToScreen(footBonePos).Y};

    //SetLootMaterial(player);
    // Draw
    if (distance > 100.f)
    {
        CleanCheat::SharedData->CurrentCanvas->K2_DrawBox(
            rootPos,
            {5.f, 5.f},
            1.0f,
            *playerDrawColor);
    }
    else
    {
        // Bones
        for (const std::vector<int>& part : _boneVector)
        {
            CG::FVector2D previousBone{};

            for (const int& bone : part)
            {
                CG::FVector2D bonePos = Utils::WorldToScreen(player->Mesh->GetBoneWorldPos(bone));
                if (bonePos.X == 0.0f && bonePos.Y == 0.0f)
                    break;

                if (previousBone.X == 0.0f) // first bone no draw happens
                {
                    previousBone = bonePos;
                    continue;
                }

                CleanCheat::SharedData->CurrentCanvas->K2_DrawLine(
                    previousBone,
                    bonePos,
                    1.0f,
                    *playerDrawColor);
                previousBone = bonePos;
            }
        }
    }

    // Text block
    // Hold Z or distance > 35
    if ((GetAsyncKeyState(0x5A) >> 15 || distance > 35) && Utils::IsInCircle(rootPos, CleanCheat::SharedData->ScreenCenterPos, _moreInfoRadius))
    {
        // Name
        {
            CleanCheat::SharedData->CurrentCanvas->K2_DrawText(
                CleanCheat::SharedData->RobotoFont,
                playerState->PlayerNamePrivate,
                textStartPos,
                CG::FVector2D(1.0f, 1.0f),
                CG::FLinearColor(0.780f, 0.137f, 0.721f, 1.0f),
                1.0f,
                CG::FLinearColor(),
                CG::FVector2D(),
                true,
                true,
                true,
                CG::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
        }

        // Distance
        {
            std::wstring distanceStr = std::to_wstring(std::round(distance));
            distanceStr.erase(distanceStr.find_last_not_of('0'), std::string::npos);
            distanceStr.append(L"M");

            CleanCheat::SharedData->CurrentCanvas->K2_DrawText(
                CleanCheat::SharedData->RobotoFont,
                distanceStr.c_str(),
                textStartPos + CG::FVector2D(0.0f, 14.0f),
                CG::FVector2D(1.0f, 1.0f),
                CG::FLinearColor(1.0f, 0.80000f, 0.40000f, 1.0f),
                1.0f,
                CG::FLinearColor(),
                CG::FVector2D(),
                true,
                true,
                true,
                CG::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
        }
    }
}

bool ChamsFeature::OnInitialize(void* initData)
{
    _boneVector =
    {
        {25, 52, 17, 54, 59, 94, 67},
        {55, 13, 14, 15, 1, 12, 8},
        {1, 107, 102}
    };

    return true;
}

void ChamsFeature::OnExecute(CG::AActor* curActor)
{
    // Rest material
    if (!CleanCheat::SharedData->GCharacter || !CleanCheat::SharedData->GCharacter->RootComponent)
    {
        CleanCheat::SharedData->LootMat = nullptr;
        return;
    }

    if (!curActor->RootComponent)
        return;

    // H Key
    if (GetAsyncKeyState(0x48) >> 15)
    {
        CG::FVector2D actorPos = Utils::WorldToScreen(curActor->RootComponent->RelativeLocation);

        if (Utils::IsInCircle(actorPos, CleanCheat::SharedData->ScreenCenterPos, _moreInfoRadius))
        {
            actorPos.Y += static_cast<float>(HKeyTestYMulti++) * 12.0f;

            CleanCheat::SharedData->CurrentCanvas->K2_DrawText(
                CleanCheat::SharedData->RobotoFont,
                S2Wc(curActor->GetFullName().c_str()),
                actorPos,
                CG::FVector2D(1, 1),
                CG::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
                1.0f,
                CG::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
                CG::FVector2D(),
                false,
                false,
                false,
                CG::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    // Player
    if (curActor->IsA(CG::AWW3Character::StaticClass()))
    {
        auto* player = reinterpret_cast<CG::AWW3Character*>(curActor);

        if (!player)
            return;

        PlayerESP(player);
        return;
    }
}

std::string ChamsFeature::Name()
{
    return "ChamsFeature";
}

bool ChamsFeature::Condition(CG::AActor* curActor)
{
    return CleanCheat::SharedData && CleanCheat::SharedData->CurrentCanvas && CleanCheat::SharedData->GWorld;
}

void ChamsFeature::BeforeExecute()
{
    HKeyTestYMulti = 0;
}

