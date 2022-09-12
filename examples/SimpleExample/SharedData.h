#pragma once
#include "CleanCheat/SharedDataBase.h"

class SharedData final : public SharedDataBase
{
public:
	/// <summary>
	/// Called every tick
	/// </summary>
	void Tick(void* data) override;
};
