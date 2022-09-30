#pragma once
#include "CleanCheat/SharedDataBase.h"

class SharedDataStruct final : public SharedDataBase
{
public:
	int MagicNumber = 50;
	
public:
	/// <summary>
	/// Called every tick
	/// </summary>
	void Tick(void* data) override;
};
