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
    virtual void OnInit() { }
};
