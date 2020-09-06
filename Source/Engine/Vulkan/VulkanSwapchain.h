#pragma once
#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanInstance;
class VulkanWindowSurface;

class VulkanSwapchain
{
public:
    API VulkanSwapchain(VulkanInstance* InInstance, VulkanDevice* InDevice, VulkanWindowSurface* InWindowSurface);

private:
    VkSwapchainKHR Swapchain;
    VulkanDevice* Device;
    VulkanWindowSurface* WindowSurface;
    VulkanInstance* Instance;

};
