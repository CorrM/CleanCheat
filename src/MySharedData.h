#pragma once
#include "CleanCheat/SharedDataBase.h"

class MySharedData final : public SharedDataBase<int>
{
public:
	int MagicNumber = 50;
	
public:
	/// <summary>
	/// Called every tick
	/// </summary>
	void Tick(int* data) override;
};
