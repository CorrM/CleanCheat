#pragma once

#define M_PI		3.14159265358979323846
#define M_PI_F		((float)(M_PI))
#define DEG2RAD(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#define RAD2DEG(x)  ((float)(x) * (float)(180.f / M_PI_F))

class Utils final
{
public:
	/// <summary>
	/// Generate random float between `a` and `b`
	/// </summary>
	/// <param name="min">Min number</param>
	/// <param name="max">Max number</param>
	/// <returns>Random float between `min` and `max`</returns>
	static float RandomFloat(float min, float max);

	static bool IsInCircle(const CG::FVector2D& point, const CG::FVector2D& circlePos, float radius);

	static void DrawCircle(CG::UCanvas* canvas, const CG::FVector2D& pos, float radius, int numSides, const CG::FLinearColor& color);
	
	/// <summary>
	/// Get current local player
	/// </summary>
	/// <returns>Current local player</returns>
	static CG::ULocalPlayer* GetLocalPlayer();
	static CG::FVector2D WorldToScreen(const CG::FVector& input, bool* outSuccess = nullptr);
	static CG::FVector2D BoneIdToScreen(const class CG::USkeletalMeshComponent* mesh, CG::APlayerController* localController, int32_t boneId, bool* success);
	static CG::FVector2D BoneIdToScreen(const class CG::ACharacter* character, CG::APlayerController* localController, int32_t boneId, bool* success);

	static bool EndsWith(const std::string& s, const std::string& suffix);
	static bool EndsWith(const std::wstring& s, const std::wstring& suffix);
};
