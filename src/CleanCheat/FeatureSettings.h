#pragma once

/// <summary>
/// Features settings class
/// </summary>
class FeatureSettings
{
public:
    bool Enable = true;

public:
    virtual ~FeatureSettings() = default;
    
protected:
    virtual bool OnInit()
    {
        return true;
    }

public:
    bool Init()
    {
        return OnInit();
    }
};
