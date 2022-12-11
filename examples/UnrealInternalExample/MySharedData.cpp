#include "pch.h"
#include "Utils.h"
#include "MySharedData.h"

void MySharedData::Tick(CG::UCanvas* canvas)
{
    CurrentCanvas = canvas;

    if (!RobotoFont)
    {
        RobotoFont = CG::UObject::FindObject<CG::UFont>("Font Roboto.Roboto");
        LOG("Roboto font: %p", RobotoFont);
    }

    GWorld = *CG::UWorld::GWorld;
    if (!GWorld)
        return;

    CG::ULocalPlayer* localPlayer = Utils::GetLocalPlayer();
    if (!localPlayer)
        return;

    GController = reinterpret_cast<CG::AShooterPlayerController*>(localPlayer->PlayerController);
    if (!GController)
        return;

    int sX = 0;
    int sY = 0;
    GController->GetViewportSize(&sX, &sY);

    ScreenSize.X = static_cast<float>(sX);
    ScreenSize.Y = static_cast<float>(sY);
    ScreenCenterPos.X = ScreenSize.X * 0.5f;
    ScreenCenterPos.Y = ScreenSize.Y * 0.5f;
    
    GCharacter = reinterpret_cast<CG::AWW3Character*>(GController->AcknowledgedPawn);

    //if (!LootMat)
    //{
    //    try
    //    {
    //        //lootMat = CG::UObject::FindObject<CG::UMaterial>("Material VertexColorViewMode_GreenOnly.VertexColorViewMode_GreenOnly");
    //        LootMat = CG::UObject::FindObject<CG::UMaterial>("Material WireframeMaterial.WireframeMaterial");
    //        if (LootMat)
    //        {
    //            LOG("Update LootMaterial");
    //            LootMat->bDisableDepthTest = 1;
    //            //LootMat->Flags = LootMat->Flags | 0x80;
    //        }
    //    }
    //    catch (...) {}
    //}
}
