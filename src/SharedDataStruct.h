#pragma once
#include "CleanCheat/SharedDataBase.h"

class SharedDataStruct final : public SharedDataBase<int>
{
public:
	int MagicNumber = 50;
	
public:
	/// <summary>
	/// Called every tick
	/// </summary>
	void Tick(int* data) override;
};
