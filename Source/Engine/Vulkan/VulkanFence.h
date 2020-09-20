#pragma once
#include "Engine/Platform/Platform.h"

class VulkanDevice;

class VulkanFence
{
public:
    API VulkanFence(VulkanDevice* InDevice);
    API ~VulkanFence();
    VkFence GetHandle() { return Fence; }
    API void Wait() const;
    API void Reset() const;

private:
    VulkanDevice* Device;
    VkFence Fence;
};