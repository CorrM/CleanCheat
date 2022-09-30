#include "pch.h"
#include <cstdlib>
#include <ctime>
#include "Utils.h"

float Utils::RandomFloat(const float min, const float max)
{
    srand(static_cast<unsigned>(time(nullptr)));
    
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

bool Utils::IsInCircle(const CG::FVector2D& point, const CG::FVector2D& circlePos, const float radius)
{
    return sqrt(pow(point.X - circlePos.X, 2) + pow(point.Y - circlePos.Y, 2)) <= (radius * M_PI_F);
}

void Utils::DrawCircle(CG::UCanvas* canvas, const CG::FVector2D& pos, float radius, const int numSides, const CG::FLinearColor& color)
{
    try
    {
        float step = M_PI_F * 2.0f / static_cast<float>(numSides);

        int count = 0;
        CG::FVector2D v[128];

        radius *= 3.2f;

        for (float a = 0; a < M_PI_F * 2.0; a += step)
        {
            float x1 = radius * cos(a) + pos.X;
            float y1 = radius * sin(a) + pos.Y;
            float x2 = radius * cos(a + step) + pos.X;
            float y2 = radius * sin(a + step) + pos.Y;

            v[count].X = x1;
            v[count].Y = y1;
            v[count + 1].X = x2;
            v[count + 1].Y = y2;

            canvas->K2_DrawLine(CG::FVector2D(v[count].X, v[count].Y), CG::FVector2D(x2, y2), 1.0f, color);
            // Circle Around
        }
    }
    catch (...) {}
}

CG::ULocalPlayer* Utils::GetLocalPlayer()
{
    CG::TArray<CG::ULocalPlayer*>& lPlayers = (*CG::UWorld::GWorld)->OwningGameInstance->LocalPlayers;
    return lPlayers[0];
}

CG::FVector2D Utils::WorldToScreen(const CG::FVector& input, bool* outSuccess)
{
    CG::FVector2D screenPos;

    CG::ULocalPlayer* lPlayer = GetLocalPlayer();
    if (!lPlayer || !lPlayer->PlayerController)
        goto Exit;

    try
    {
        bool w2ScreenSuccess = lPlayer->PlayerController->ProjectWorldLocationToScreen(input, &screenPos, false);

        if (outSuccess)
            *outSuccess = w2ScreenSuccess;

        return screenPos;
    }
    catch (...) {}

Exit:
    if (outSuccess)
        *outSuccess = false;

    return screenPos;
}

CG::FVector2D Utils::BoneIdToScreen(const CG::USkeletalMeshComponent* const mesh, CG::APlayerController* const localController, const int32_t boneId, bool* success)
{
    CG::FVector2D screenLoc;
    if (success)
        *success = false;

    if (!mesh || !localController)
        return screenLoc;

    CG::FVector worldPos = mesh->GetBoneWorldPos(boneId);
    bool toWorld = localController->ProjectWorldLocationToScreen(worldPos, &screenLoc, true);
    if (success)
        *success = toWorld;

    return screenLoc;
}

CG::FVector2D Utils::BoneIdToScreen(const CG::ACharacter* const character, CG::APlayerController* localController, const int32_t boneId, bool* success)
{
    if (!character || !localController || !character->Mesh)
        return {0.f, 0.f};

    return BoneIdToScreen(character->Mesh, localController, boneId, success);
}

bool Utils::EndsWith(const std::string& s, const std::string& suffix)
{
    return s.length() >= suffix.length()
               ? (0 == s.compare(s.length() - suffix.length(), suffix.length(), suffix))
               : false;
}

bool Utils::EndsWith(const std::wstring& s, const std::wstring& suffix)
{
    return s.length() >= suffix.length()
               ? (0 == s.compare(s.length() - suffix.length(), suffix.length(), suffix))
               : false;
}
