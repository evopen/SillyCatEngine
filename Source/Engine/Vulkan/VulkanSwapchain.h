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

    API VkSwapchainKHR GetSwapchainHandle() { return Swapchain; }
    API VkImage GetImage(uint32_t Index) { return Images[Index]; }
    API uint32_t GetRenderIndex() const { return RenderIndex; }
    API uint32_t GetPresentIndex() const { return PresentIndex; }

private:
    VkSwapchainKHR Swapchain;
    VulkanDevice* Device;
    VulkanWindowSurface* WindowSurface;
    VulkanInstance* Instance;
    std::vector<VkImage> Images;

    uint32_t RenderIndex;
    uint32_t PresentIndex;
};
