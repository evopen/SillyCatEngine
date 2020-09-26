#pragma once

class VulkanDevice;

class Shading
{
public:
    Shading(VulkanDevice* inDevice)
        : Device(inDevice){};

protected:
    VulkanDevice* Device;
};
