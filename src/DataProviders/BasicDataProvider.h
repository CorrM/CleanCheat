#pragma once
#include "CleanCheat/DataProviderBase.h"

class BasicDataProvider final : public DataProviderBase<int, std::string>
{
private:
    std::string _secretName = "CleanCheat";
    
protected:
    void OnExecute(int* param) override;
    
public:
    std::string Name() override;
    bool Condition(int* param) override;
};
