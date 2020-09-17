#pragma once
#include "Engine/Platform/Platform.h"

class VulkanDevice;

class VulkanFence
{
public:
    API VulkanFence(VulkanDevice* InDevice);
    VkFence* GetHandle() { return &Fence; }

private:
    VulkanDevice* Device;
    VkFence Fence;
};